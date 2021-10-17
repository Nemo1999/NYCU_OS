import subprocess
import matplotlib.pyplot as plt
import numpy as np


def test(mode='single_thread', arr_size=10000000, num_thread=None, num_proc=None, num_samples=10,  target_num=0):
    cmd = []
    program_name = './single_thread' if mode == 'single_thread' else ('./process' if mode=='multi_process' else './thread')
    cmd.append(program_name)
    cmd.append(str(arr_size))
    cmd.append(str(target_num))
    if mode == 'multi_process':
        cmd.append(str(num_proc))
    if mode == 'multi_thread':
        cmd.append(str(num_thread))
    total_usecs = 0
    for i in range(num_samples):
        p = subprocess.run(cmd, capture_output=True, check=True, text=True)
        words = p.stdout.split(' ')
        u_secs = int(words[-2]) + int(words[-5]) * (10**6)
        total_usecs += u_secs
    return total_usecs / num_samples

if __name__ == '__main__':


    # start of set parameters -------------------------------------------------------------------
    # number of experiments to run for each test (result is averaged)
    num_sample = 5
     # number of value to test for array_size
    num_x_vals = 11
    # start of range of array_size
    x_start = 0
    # end of range of array_size
    x_end = 50000000
    # number of different thread numbers to try  (try powers of 2 start from 2**0)
    num_thread_setting = 5
    # end of set parameters ------------------------------------------------------------------

    # execute experiments
    mode = 'multi_process' # don't change this,  this script for multi_process
    times = [[] for _  in range(num_thread_setting)]
    xs = []
    for arr_size in np.linspace(x_start, x_end, num_x_vals):
        arr_size = int(arr_size)
        xs.append(int(arr_size))
        print(f'trying arr_size = {arr_size}')
        for j in range(num_thread_setting):
            num_processes = int(2 ** j)
            t = test(mode, arr_size, None , num_processes, num_sample)
            times[j].append(t)
            print(f'\tprocess: {num_processes:4.1f}, time: {t:10.1f} (uS)')

    # plot figure
    fig, ax = plt.subplots(figsize=(7,5))
    ax.set_title('Multi_processes Experiments')
    for i in range(num_thread_setting):
        percent = (i/num_thread_setting)
        r = 0.0 + 0.9 * percent
        g = 0.1
        b = 0.9 - 0.9 * percent
        color = (r, g, b, 1.0)
        ax.plot(xs, times[i], color=color , label=f'{2**i} processes')
    ax.set_xlabel('array size')
    ax.set_ylabel('time (us)')
    ax.grid()
    ax.legend()
    plt.savefig(f'process.png')
    plt.plot()
