# Matrix Multiplication

* Thread Matrix Multiplication: The input to the program is two matrixes A(x*y) and B(y*z) that are read from corresponding files.
  The output is a matrix C(x*z) that is written to an output file. A parallelized version of matrix multiplication can be
  done using one of these two methods: (1) a thread computes each row in the output C matrix, or (2)
  a thread computes each element in the output C matrix.

* Block Matrix Multiplication : We will be running 8 trials of our experiments, for each trial we will run 4
  different experiment. Our varying factor will be the block_size. For the first
  experiment we will start with a 256 block size, for each successive experiment
  we will double the block size. We continue doing this until reaching a maximum
  block size of 2048 (The same size as our matrix).

## Authors
* **Mohamed Bakr** [MohamedBakrAli](https://github.com/MohamedBakrAli)
* **Ahmed Hamdy** [Hamdy10024](https://github.com/Hamdy10024)
## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
