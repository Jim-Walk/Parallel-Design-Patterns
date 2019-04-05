#!/usr/bin/python3
import re, codecs, os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import sys


# first strip out checks and average files
# place values into a dictionary of data frames
# where d[schedule + n_threads] = data frame

f = sys.argv[1]

df = pd.read_csv(f,index_col=0)
print(df)

plt.figure()
axes = plt.axes()
#axes.set_xlabel("Time (seconds)")
axes.set_ylabel("Count")
#axes.set_ylim([0,6])
#axes.set_xticks([1,2,4,6,8,12,16])
#axes.set_yticks([2,4,6,8,10,12])
p = df.plot(ax=axes, marker='.').get_figure()
p.savefig(f.split('.')[0])
