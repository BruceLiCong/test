int problem1(int a, int b, int n)
{
	int aC = ((n - 1)/ a);
	int bC = ((n - 1)/ b);
	int abC = ((n - 1)/ (a * b));

	return ((a * aC * (aC + 1)) + (b * bC * (bC + 1)) - (a * b * abC * (abC + 1))) >> 1;
}

int main()
{
	return problem1(3, 5, 1000);
}