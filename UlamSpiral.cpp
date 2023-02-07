#include <iostream>
#include <fstream>
#include <chrono>
#include <string>

// Ulam Spiral Creator

// Change bit to 0 in unsigned char integer
void set_bit_false(unsigned char& a, const unsigned char& B)
{
	const unsigned char C = 1;
	a &= ~(C << B);
}

// Check bit in unsigned char integer
const bool decode_bit(const unsigned char& A, const unsigned char& B)
{
	const unsigned char C = 1;
	return A & (C << B);
}

int main()
{
	// Input n x n dimension of Ulam Spiral
	long long int SIZEBYSIZE;

	std::cout << "Input Ulam Spiral Size: ";
	std::cin >> SIZEBYSIZE;

	// Set beginning time of operations
	auto begin = std::chrono::high_resolution_clock::now();

	const long long int PRIMESIZE = SIZEBYSIZE * SIZEBYSIZE;
	const long long int ACTUALPRIMESIZE = PRIMESIZE / 16 + 1;

	std::cout << "\nBUILDING PRIME ARRAY\n";

	// Build dynamic array of unsigned char type integers whose bits indicate a prime number
	// The location of a prime number bit in any unsigned char type integer within the primes array is floor(x / 8) for the integer itself, and x mod 8 for the bit

	unsigned char* primes = new unsigned char[ACTUALPRIMESIZE];

	for (long long int i = 0; i < ACTUALPRIMESIZE; i++)
	{
		primes[i] = -1;
	}

	std::cout << "\nASSIGNING PRIMES INTO PRIME ARRAY\n";

	// Sieve Of Eratosthenes

	set_bit_false(primes[0], 0); // 1

	long long int curprime = 3;

	while (curprime * curprime <= PRIMESIZE)
	{
		for (long long int i = curprime * curprime; i <= PRIMESIZE; i += curprime * 2)
		{
			set_bit_false(primes[(i - 1) / 16], ((i - 1) / 2) % 8);
		}
		curprime += 2;
		while (!decode_bit(primes[(curprime - 1) / 16], ((curprime - 1) / 2) % 8))
		{
			curprime += 2;
		}
	}

	std::cout << "\nBUILDING SPIRAL USING PRIME ARRAY\n";

	// Build the Ulam Spiral based on the primes calculated and the polynomial properties of the Ulam Spiral
	//  We use two polynomials to calculate all the numbers:
	//   
	//  4x^2 + (SIZEBYSIZE mod 2) + i - j                if j is less than SIZEBYSIZE - i
	// 
	//  4x^2 + 4x + 2 - (SIZEBYSIZE mod 2) + j - i       otherwise
	//   
	//  -> i is the current row index
	//  -> j is the current column index
	//  -> x is the chebyshev distance between (i, j) and (CENTERROW, CENTERCOL) if j is less than SIZEBYSIZE - i,
	//	   or (i, j) and (CENTERROW, CENTERCOL + 1) otherwise

	std::ofstream ostr("ulamspiral_" + std::to_string(SIZEBYSIZE) + ".pbm");

	// PBM Header
	ostr << "P1\n";

	// Width, Height
	ostr << SIZEBYSIZE << " " << SIZEBYSIZE << "\n";

	const long long int SIZEMOD2 = SIZEBYSIZE % 2;
	const long long int CENTERROW = SIZEBYSIZE / 2;
	const long long int CENTERCOL = SIZEBYSIZE / 2 - 1 + SIZEMOD2;

	char* ulam_segment = new char[SIZEBYSIZE];
	bool not_two = true;

	for (long long int i = 0; i < SIZEBYSIZE; i++)
	{
		const long long int ROW_I = abs(i - CENTERROW);
		const long long int DIFF_SIZE_I = SIZEBYSIZE - i;

		for (long long int j = (i + SIZEMOD2) % 2; j < SIZEBYSIZE; j += 2)
		{
			ulam_segment[j] = 48;
		}

		for (long long int j = (i + 1 - SIZEMOD2) % 2; j < SIZEBYSIZE; j += 2)
		{
			if (j < DIFF_SIZE_I)
			{
				const long long int COL_J = abs(j - CENTERCOL);
				const long long int DIST = ROW_I >= COL_J ? ROW_I : COL_J;

				const long long int NUM = 4 * DIST * DIST + SIZEMOD2 + i - j;

				ulam_segment[j] = 48 + decode_bit(primes[(NUM - 1) / 16], ((NUM - 1) / 2) % 8);
			}
			else
			{
				const long long int COL_J = abs(j - CENTERCOL - 1);
				const long long int DIST = ROW_I >= COL_J ? ROW_I : COL_J;

				const long long int NUM = 4 * DIST * DIST + 4 * DIST + 2 - SIZEMOD2 + j - i;

				ulam_segment[j] = 48 + decode_bit(primes[(NUM - 1) / 16], ((NUM - 1) / 2) % 8);
			}
		}

		if (not_two && SIZEBYSIZE > 1 && i == CENTERROW)
		{
			ulam_segment[CENTERCOL + 1] = 49;
			not_two = false;
		}

		ostr.write(ulam_segment, SIZEBYSIZE);
	}

	delete[] ulam_segment;
	ulam_segment = nullptr;

	ostr.close();

	std::cout << "\nDELETING PRIME ARRAY\n";

	// Delete and make nullptr primes as it's not needed anymore

	delete[] primes;
	primes = nullptr;

	// Set end time of operations
	auto end = std::chrono::high_resolution_clock::now();

	std::cout << "\n" << SIZEBYSIZE << "x" << SIZEBYSIZE << " Ulam Spiral successfully built\n\n";
	std::cout << "Runtime: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms\n\n\n";

	std::cout << "Press any key then Enter to close program\n";
	std::cin >> SIZEBYSIZE;

	return 0;
}
