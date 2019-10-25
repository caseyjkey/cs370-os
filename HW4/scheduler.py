from collections import deque
import copy
from operator import attrgetter
import sys

import pandas

toDebug =True 

class Process:
    def __init__(self, process):
        self.id = process['ProcessID'].item()
        self.arrival = process['Arrival Time'].item()
        self.burst = process['Burst Time'].item()
        self.priority = process['Priority'].item()
        
        self.start_times = deque() 
        self.end_times = deque()
        self.preemptions = 0
        self.total_wait_time = 0
        self.wait_start = 0 
        self.wait_end = None
        self.time_idle = 0

        self.turnaround = None
    
    def start(self, start):
        # RR requires multiple starts
        if start not in self.start_times:
            self.start_times.append(start)

    def wait(self, current_time):
        self.wait_start = current_time
        if self.start_times:
            self.preemptions += 1

    def finish(self, end):
        # RR requires a process to stop multiple times
        self.end_times.append(end)
        # Turnaround will be when process finished - arrival
        self.turnaround = self.end_times[-1] - self.arrival

    def finish_idle(self, startup):
        self.time_idle += startup - self.wait_start
    
    def __str__(self):
        return f'({self.id}, {self.burst})'#f'{self.id}: Arrived at {self.arrival} {self.start} to {self.end} Burst: {self.burst} Wait Start: {self.wait_start}'

# Used for Round Robin Gantt Charting
# Can be replaced by Process with overloaded initializer
class SimpleProcess:
    def __init__(self, pid, start, end):
        self.start_times = deque([start])
        self.end_times = deque([end])
        self.id = pid


def debug(output):
    if(toDebug):
        print(output, end='')

def get_processes_df_from_file(csv_file):
    processes = pandas.read_csv(open(csv_file))
    processes = processes.sort_values('ProcessID').sort_values('Arrival Time')

    return processes

# First come first serve assuming processes are sorted by arrival time
def FCFS(processes_df):
    processes = []
    
    currentTime = 0
    for i, row in processes_df.iterrows():
        process = Process(row)
        process.total_wait_time = max(currentTime - process.arrival, 0)

        # Include time when IDLE
        if process.arrival > currentTime:
            process.wait_start = currentTime
            # idle_result = {'pid': 'IDLE', 'begin': currentTime, 'end': arrival}
            # alg_result.append(idle_result)
            process.finish_idle(process.arrival)
            currentTime = process.arrival

        process.start(currentTime)
        currentTime += process.burst 
        process.finish(currentTime)

        processes.append(process)

    processes = sorted(processes, key=attrgetter('total_wait_time', 'id'))
    return processes


def PBS(processes_df):
    processes = []

    # Assume we are sorted by Arrival -> PID
    processes_df = processes_df.sort_values(['Arrival Time', 'Priority'])
    currentTime = 0
    for i, row in processes_df.iterrows():
        process = Process(row)
        process.total_wait_time = max(currentTime - process.arrival, 0)
        
        # Include time when IDLE
        if process.arrival > currentTime:
            process.wait_start = currentTime
            process.finish_idle(process.arrival)
            currentTime = process.arrival

        process.start(currentTime)
        currentTime += process.burst 
        process.finish(currentTime)

        processes.append(process)

    processes = sorted(processes, key=attrgetter('total_wait_time', 'id'))
    return processes

def RR(processes_df, time_quantum):
    processes = []
    processes_df = processes_df.sort_values(['Arrival Time', 'ProcessID'])

    current_time = 0
    # deque for fast appends and poplefts
    queue = deque()
    for i, row in processes_df.iterrows():
        process = Process(row)
        print("Current process: ", process.id, "at time", current_time)
        debug([(process.id, process.burst) for process in queue])
        
        # take care of items in queue before working on new items
        if queue:
            process.wait(current_time)
            # push new process to queue if last item has been waiting
            if queue[-1].wait_start < process.arrival:
                queue.append(process)
            # new process arrived as previous process was context switched
            else: 
                # give priority to new process over context switched process
                queue.insert(-1, process)

            # proceed working on the first process in line
            process = queue.popleft()
            process.finish_idle(current_time)

        # No processes, must idle
        if(process.arrival >= current_time):
            # set start time and begin executing
            process.start(process.arrival)
            current_time = process.arrival
        
        # Start if this is a fresh process
        if not process.start_times:
            process.start(current_time)
            
        # execute process for given time quantum
        work_units = time_quantum if process.burst >= time_quantum else process.burst
        q = str([(process.id, process.burst) for process in queue])
        debugOutput = f' working on: {str(process)} for {str(work_units)} units {q: ^10}'
        debug(debugOutput)
        debug("\n")
         
        process.start(current_time)
        result =  process.burst - time_quantum
        if result > 0:
            # execute for maximum allowed (time quantum)
            current_time += time_quantum
            process.burst -= time_quantum
            # context switch the process
            process.wait(current_time)
            process.finish(current_time)
            queue.append(process)
            

           
           
        else:
            # finish executing
            current_time += process.burst
            process.finish(current_time)
            work_units = process.burst
            q = str([(process.id, process.burst) for process in queue])
            debugOutput = f' finished process {str(process.id)} in {str(work_units)} units {q: ^10}'
            debug(debugOutput)
            debug("\n")
            # add finished process to results
            processes.append(process)

    # all processes are queued or completed
    while(queue):
        process = queue.popleft()
        process.finish_idle(current_time)
        print("Current process: ", process.id, "at time", current_time)
        debug([(process.id, process.burst) for process in queue])
        
        # No processes, must idle
        if(process.arrival >= current_time):
            # set start time and begin executing
            process.start(process.arrival)
            current_time = process.arrival
        
        if not process.start_times:
            process.start(current_time)
        # execute process for given time quantum
        work_units = time_quantum if process.burst >= time_quantum else process.burst
        q = str([(process.id, process.burst) for process in queue])
        debugOutput = f' working on: {str(process)} for {str(work_units)} units {q: ^10}'
        debug(debugOutput)
        debug("\n")
         
        result =  process.burst - time_quantum
        process.start(current_time)
        if result > 0:
            # execute for maximum allowed (time quantum)
            current_time += time_quantum
            process.burst -= time_quantum
            # context switch the process
            # begin waiting
            process.wait(current_time)
            # set end time
            process.finish(current_time)
            queue.append(process)
            
        else:
            # finish executing
            current_time += process.burst
            process.finish(current_time)
            work_units = process.burst
            q = str([(process.id, process.burst) for process in queue])
            debugOutput = f' finished process {str(process.id)} in {str(work_units)} units {q: ^10}'
            debug(debugOutput)
            debug("\n")
            # add finished process to results
            processes.append(process)

    for process in processes:
        print(f'------- {process.id} ------')
        for i in range(len(process.start_times)):
            print(f'{process.start_times[i]}', end = " ")
        print()
        for i in range(len(process.end_times)):
            print(f'{process.end_times[i]}', end = " ")
        print()
    # processes ordered by finish time
    print(len(processes))
    return processes

            
        
def printSchedulingTable(alg_name, processes):
    # Go in order of PID
    processes = sorted(processes, key=attrgetter('id'))
    # Display header
    print('-' * 17, alg_name, '-' * 17)
    # Column names
    print('Process ID | Waiting Time | Turnaround Time')
    # Make chart
    for process in processes:
        print('{: ^10} |{: ^14}|{: ^17}'.format(process.id, process.total_wait_time, process.turnaround))

def printGanttChart(processes_orig):
    # Go in order of which process finishes first
    processes = copy.deepcopy(processes_orig)
    processes = sorted(processes, key = attrgetter('end_times'))
    previousEnd = 0
    print('Gantt Chart is:')
    for process in processes:
        if not process.end_times:
            continue
    
        # Check for IDLE time
        if process.start_times[0] != previousEnd:
            pid = 'IDLE'
            print(f'[{previousEnd: ^6}]--{pid: ^6}--[{process.start_times[0]: ^6}]')       

        print(f'[{process.start_times.popleft(): ^6}]--{process.id: ^6}--[{process.end_times[0]: ^6}]') 
        previousEnd = process.end_times.popleft()

# Because RR preempts, we may need more than one entry per process
# For each start/stop time, we will add a new process
def expandProcessesList(processes):
    expandedProcesses = []
    for process in processes:
        pid = process.id
        for (start, end) in zip(process.start_times, process.end_times):
            expandedProcesses.append(SimpleProcess(pid, start, end))
    return expandedProcesses


def printStats(processes):
    total_wait = sum([process.total_wait_time for process in processes])
    avg_wait = total_wait/len(processes)

    total_turnaround = sum([process.turnaround for process in processes])
    avg_turnaround = total_turnaround/len(processes)

    # Throughput = Number of Processes/Total Time
    # Get total time
    processes = sorted(processes, key = attrgetter('end_times'))
    total_time = processes[-1].end_times[-1]
    # Length of result is equal to number of processes
    throughput = len(processes)/total_time             

    print(f'Average Waiting Time: {avg_wait}')
    print(f'Average Turnaround Time: {avg_turnaround}')
    print(f'Throughput: {throughput:.12f}')

if __name__ == "__main__":
    argv = sys.argv
    processes = argv[1]
    time_quantum = int(argv[2])
    toDebug = bool(argv[3])
    processes = get_processes_df_from_file(processes)
    result = FCFS(processes)
    printSchedulingTable("FCFS", result)
    print()
    printGanttChart(result)
    print()
    printStats(result)
    print()

    result = PBS(processes)
    printSchedulingTable("PBS", result)
    print()
    printGanttChart(result)
    print()
    printStats(result)
    print()

    result = RR(processes, time_quantum)
    printSchedulingTable("Round Robin", result)
    print()
    expandedResult = expandProcessesList(result)
    printGanttChart(expandedResult)
    print()
    printStats(result)
