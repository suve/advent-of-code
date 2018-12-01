import java.util.Scanner;

class Freq {
	public static void main(String[] args) {
		int freq, change;
		freq = 0;
		
		Scanner s = new Scanner(System.in);
		while (s.hasNextInt()) {
			change = s.nextInt();
			freq += change;
		}
		
		System.out.println(freq);
	}
}

