object "Application"
{
	DESIRED_SUM = 2020;
	go_on = true;

	state "main"
	{
		numbers = {};
		while(go_on)
		{
			line = Console.readline();
			if(line == null)
			{
				Console.print("No suitable numbers found");
				go_on = false;
			}

			n1 = Number(line);
			numbers.set(n1, true);

			n2 = DESIRED_SUM - n1;
			if(numbers.has(n2))
			{
				mul = n1 * n2;
				Console.write("Result: ");
				Console.print(mul);
				go_on = false;
			}
		}
	}
}
