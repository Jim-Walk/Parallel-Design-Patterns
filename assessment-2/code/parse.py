#!/usr/bin/python3
# A simple script to parse the from the program and generate a csv file
# for plotting
# Output file format: month, population avg, infection avg, live squirrels,
# infected squirrels

# Usage ./parse.py $filetoparse > out.csv

import sys

pops_by_month, infs_by_month, live_sq, inf_sq = [[]], [[]] ,[], []


# Relies on list index only ever being one more than the item
def add_to_list(lst, index,item):
    if len(lst) <= index:
        lst += [[item]]
    else:
        lst[index] += [item]

def avg_list(lst):
    if lst != [] or lst != [0]:
        return sum(lst)/len(lst)


with open(sys.argv[1]) as f:
    for line in f.readlines():
        line = line.strip().split()
        if line[0] == 'gc':
            if line[2] == 'month:':
                month = int(line[3])
                new_pop = int(line[5].split('.')[0])
                new_inf = int(line[7].split('.')[0])
                #print(month,new_pop_avg,new_inf_avg)
                add_to_list(pops_by_month, month, new_pop)
                add_to_list(infs_by_month, month, new_inf)
            else:
                month  += 1
                new_pop = int(line[4].split('.')[0])
                new_inf = int(line[6].split('.')[0])
                add_to_list(pops_by_month, month, new_pop)
                add_to_list(infs_by_month, month, new_inf)

        if line[0] == 'C17:':
            if line[4] != 'Infected':
                live_sq += [int(line[4])]
                inf_sq += [int(line[6])]
            else:
                live_sq += [int(line[3])]
                inf_sq += [int(line[6])]

    avg_infs = []
    avg_pops = []
    # Average population and infection levels for all gridcells
    i = 1
    while i < len(infs_by_month):
        avg_infs += [avg_list(infs_by_month[i])]
        avg_pops += [avg_list(pops_by_month[i])]
        i += 1

    # Print output
    print("Months, Average Population, Average Infection, Live Squirrels, Infected Squirrels")
    i = 0
    while i < len(live_sq):
        print(i+1,",",avg_pops[i], ",",avg_infs[i], ",",live_sq[i], ",",inf_sq[i])
        i += 1

