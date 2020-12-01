object "Application"
{
	DESIRED_SUM = 2020;

	state "main"
	{
		numbers = [];
		loop_input = true;
		while(loop_input)
		{
			line = Console.readline();
			if(line != null)
			{
				numbers.push(Number(line));
			}
			else
			{
				loop_input = false;
			}
		}

		for(i = 0; i < numbers.length; i += 1)
		{
			n_i = numbers.get(i);
			for(j = i+1; j < numbers.length; j += 1)
			{
				n_j = numbers.get(j);
				for(k = j+1; k < numbers.length; k += 1)
				{
					n_k = numbers.get(k);
					if(n_i + n_j + n_k == DESIRED_SUM)
					{
						Console.write("Result: ");
						Console.print(n_i * n_j * n_k);
					}
				}
			}
		}
		System.exit();
	}
}
