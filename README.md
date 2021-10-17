# NYCU_OS HW1

## 1. To Build C files, use:  
```bash
make compile
```
Compile three c files `single_thread.c`, `process.c`, and `thread.c`. 
- **`single_thread`** takes 2 arguments: `arr_size` and `target_number`
- **`process`** takes 3 arguments: `arr_size`, `target_number`, and `num_processes`
- **`thread`** takes 3 arguments: `arr_size`, `target_number`, and `num_threads`
For example, you can run **`thread`** with
```bash
./thread 10000000 0 4
```
This will run `4` threads on an randomly generated array of size `10000000` to count the number occurance of `0`  
Test results:
![](https://i.imgur.com/KxlC44X.png)


## 2. To RUN Experiments and plot results, use:
```
make plot 
```
This will runs `plot_thread.py` and `plot_process.py` repectively
These two scripts use **subprocess** library to execute compiled c files mentioned in step 1.
You can change the parameter settings in the files, include:
- `num_sample`: number of repeated experiments to run, the timing results are averaged
- `num_x_vals`: number of array_sizes to try (linearly interpolated between larges and smallest size)
- `x_start`: smallest array_size
- `x_end`:largest array_size
- `num_thread_settings`: number of thread settings to try (we try powers of 2 starting from 2^0)
The result on my laptop is shown below:
![](https://i.imgur.com/Iz8Eh4w.png)
![](https://i.imgur.com/U9Wg2N2.png)

