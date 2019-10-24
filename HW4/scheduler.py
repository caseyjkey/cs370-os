from  operator import itemgetter 
import sys

import pandas


argv = sys.argv

processes = argv[1]
time_quantum = argv[2]


def get_processes_df_from_file(csv_file):
    processes = pandas.read_csv(open(csv_file))
    processes = processes.sort_values('ProcessID').sort_values('Arrival Time')
    
    return processes

# Assume processes are sorted by arrival time
def FCFS(processes_df):
    alg_result = []

    currentTime = 0 
    for i, row in processes_df.iterrows():
        pid = row['ProcessID']
        arrival = row['Arrival Time']
        wait = max(currentTime - arrival, 0)

        # Include time when IDLE                
        if arrival > currentTime:
                #idle_result = {'pid': 'IDLE', 'begin': currentTime, 'end': arrival}
                #alg_result.append(idle_result)
                currentTime = arrival

        start = currentTime
        currentTime += row['Burst Time']
        end = currentTime
        turnaround = end - arrival      

        result = {'pid': pid, 'begin': start, 'end': end, 'wait': wait, 'turnaround': turnaround}
        alg_result.append(result)

    alg_result = sorted(alg_result, key = itemgetter('wait', 'pid'))
    return alg_result

def PBS(processes_df):
    alg_result = []

    # Assume we are sorted by Arrival -> PID 
    processes_df = processes_df.sort_values(['Arrival Time', 'Priority'])
    currentTime = 0 
    for i, row in processes_df.iterrows():
        pid = row['ProcessID']
        arrival = row['Arrival Time']
        wait = max(currentTime - arrival, 0)

        # Include time when IDLE                
        if arrival > currentTime:
                #idle_result = {'pid': 'IDLE', 'begin': currentTime, 'end': arrival}
                #alg_result.append(idle_result)
                currentTime = arrival

        start = currentTime
        currentTime += row['Burst Time']
        end = currentTime
        turnaround = end - arrival      

        result = {'pid': pid, 'begin': start, 'end': end, 'wait': wait, 'turnaround': turnaround}
        alg_result.append(result)

    alg_result = sorted(alg_result, key = itemgetter('wait', 'pid'))
    return alg_result
        

def printSchedulingTable(alg_name, alg_result):
    alg_result = sorted(alg_result, key = itemgetter('pid'))        
    print('-' * 17, alg_name, '-' * 17)
    print('Process ID | Waiting Time | Turnaround Time')
    for process in alg_result:
        pid = process['pid']
        wait = process['wait']
        turnaround = process['turnaround']

        print('{: ^10} |{: ^14}|{: ^17}'.format(pid, wait, turnaround))

def printGanttChart(result_array):
    result_array = sorted(result_array, key = itemgetter('end'))
    previousEnd = 0
    print('Gantt Chart is:')
    for process in result_array:
        begin = process['begin']
        end = process['end']    
        # Check for IDLE time
        if begin != previousEnd:
            pid = 'IDLE'
            print(f'[{previousEnd: ^6}]--{pid: ^6}--[{begin: ^6}]')     

        pid = process['pid']

        print(f'[{begin: ^6}]--{pid: ^6}--[{end: ^6}]') 
        previousEnd = end


def printStats(alg_result):
    total_wait = sum([process['wait'] for process in alg_result])
    avg_wait = total_wait/len(alg_result)

    total_turnaround = sum([process['turnaround'] for process in alg_result])
    avg_turnaround = total_turnaround/len(alg_result)
    
    # Throughput = Number of Processes/Total Time
    # Get total time
    alg_result = sorted(alg_result, key = itemgetter('end'))
    total_time = alg_result[-1]['end']
    # Length of result is equal to number of processes
    throughput = len(alg_result)/total_time         

    print(f'Average Waiting Time: {avg_wait}')
    print(f'Average Turnaround Time: {avg_turnaround}')
    print(f'Throughput: {throughput:.12f}')

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
