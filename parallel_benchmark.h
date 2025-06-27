#pragma once


bool is_prime(long long n);
void find_primes_in_range(long long start, long long end, std::atomic<long long>& prime_count);
void run();
