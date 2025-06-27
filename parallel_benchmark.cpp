#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <atomic> // For safe shared counter


namespace parallel {

// Function to check if a number is prime
bool is_prime(long long n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (long long i = 5; i * i <= n; i = i + 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

// Worker function for each thread to find primes in a range
void find_primes_in_range(long long start, long long end, std::atomic<long long>& prime_count) {
    for (long long i = start; i <= end; ++i) {
        if (is_prime(i)) {
            prime_count++;
        }
    }
}

void run() {
    std::cout << "------------------------" << std::endl;

    // Determine the number of hardware threads available
    unsigned int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) {
        std::cerr << "Warning: Could not determine number of hardware threads. Defaulting to 4." << std::endl;
        num_threads = 4; // Fallback
    }
    std::cout << "Using " << num_threads << " threads for benchmarking." << std::endl;

    long long max_number_to_check = 2000000; // Adjust as needed for desired duration
    std::cout << "Finding primes up to " << max_number_to_check << std::endl;

    std::atomic<long long> total_primes_found(0);
    std::vector<std::thread> threads;

    // Divide the workload among threads
    long long chunk_size = max_number_to_check / num_threads;

    auto start_time = std::chrono::high_resolution_clock::now();

    for (unsigned int i = 0; i < num_threads; ++i) {
        long long start_range = i * chunk_size + 1;
        long long end_range = (i == num_threads - 1) ? max_number_to_check : (i + 1) * chunk_size;
        
        threads.emplace_back(find_primes_in_range, start_range, end_range, std::ref(total_primes_found));
    }

    // Join all threads to wait for their completion
    for (std::thread& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;

    std::cout << "\nMulticore CPU Benchmark Results:" << std::endl;
    std::cout << "Total primes found: " << total_primes_found.load() << std::endl;
    std::cout << "Total execution time: " << duration.count() << " seconds" << std::endl;
    std::cout << "Effective operations per second (approx): " << (double)max_number_to_check / duration.count() << std::endl;

    std::cout << "------------------------\n" << std::endl;
}

} // namespace parallel
