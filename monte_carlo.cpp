// MPI_PIMonteCarlo.cpp
#include <mpi.h>
#include <cassert>
#include <random>
#include <iomanip>
#define DEFAULT_ITERATIONS_COUNT 1000
#define LOOPS_IN_ITERATION 100000
#define TASK_TAG 1024
using namespace std;
mt19937 mt_engine;
uniform_real_distribution<double> real_distribution;
long calculate_hits(int iterations);
void perform_process(long total_iterations, int current_rank, int world_size);
// Main entry point to the program.
//
// Specifying a number argument allows to control number of iterations done in the
program
// $1 - number of iteration to be performed
int main(int argc, char* argv[])
{
// Determine the number of iterations to be done using user input
long total_iterations = 0;
if (argc == 2)
{
char *input = argv[1];
total_iterations = strtol(input, NULL, 10);
}
if (total_iterations == 0)
{
total_iterations = DEFAULT_ITERATIONS_COUNT;
}
// Randomize
random_device rd;
mt_engine.seed(rd());
// Initialize MPI and basic variables
MPI_Init(&argc, &argv);
double tic = MPI_Wtime();
int world_size;
MPI_Comm_size(MPI_COMM_WORLD, &world_size);
int current_rank;
MPI_Comm_rank(MPI_COMM_WORLD, &current_rank);
// Run the main process
perform_process(total_iterations, current_rank, world_size);
// Finish and produce summary
double toc = MPI_Wtime();
MPI_Finalize();
if (current_rank == 0)
{
printf("--- Operation took %f seconds ---\n", (toc - tic));
system("pause"); } return 0; } void perform_process(long total_iterations, int current_rank, int world_size) { // Start of the process printf( "[Node #%f] Starting process of calculating pi value.\n", current_rank); int *iterations_distribution = new int[world_size]; if (current_rank == 0) { int iterations_left = total_iterations; int iterations_per_worker = total_iterations / world_size; // Fill in a number of loops except for the last worker for (int i = 0; i < world_size - 1; i++) { iterations_distribution[i] = iterations_per_worker; iterations_left -= iterations_per_worker; } // Last worker gets the remaining loops to do iterations_distribution[world_size - 1] = iterations_left; printf("[Node #0] Base nodes iterations: %f last node: %f\n",iterations_per_worker, iterations_left); } // Distribute and perform the work int iterations; MPI_Scatter(iterations_distribution, 1, MPI_INT, &iterations, 1, MPI_INT, 0, MPI_COMM_WORLD); printf("[Node #%f] Got %f iterations to perform.\n",current_rank,iterations); // Perform the work and collect the result long long total_hits = 0; for (int i = 0; i < iterations; i++) { int result = calculate_hits(LOOPS_IN_ITERATION); total_hits += result; if (current_rank == 0 && i % 256 == 0) { printf("[Node #0] %f % done\n",(100 * i) / iterations); } } // Gather incoming responses long long *received_hits = new long long[world_size]; MPI_Gather(&total_hits, 1, MPI_LONG_LONG, received_hits, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD); if (current_rank == 0) { long long total_received_hits = 0; for (int i = 0; i < world_size; i++) { total_received_hits += received_hits[i]; } double total_attempts = (double)total_iterations * LOOPS_IN_ITERATION; double pi_value = (double)total_received_hits * 4.0 / total_attempts; printf("[Node #0] Received %f received hits in %f attempts.\n", total_received_hits, total_attempts); printf("--------------------------------------------\n") printf("[Node #0] Calculated PI value is %f\n",pi_value);
printf("[Node #0] Difference is %f\n\n",3.14159265358979323846 - pi_value); } printf("[Node # %f] dies.\n",current_rank); // Clean up delete[] iterations_distribution; delete[] received_hits; } long calculate_hits(int iterations) { int i; int hits = 0; for (i = 0; i < iterations; i++) { double x = (double)real_distribution(mt_engine); double y = (double)real_distribution(mt_engine); if (x * x + y * y <= 1.0) { hits++; } } return hits; }
