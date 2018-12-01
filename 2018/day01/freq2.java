import java.util.ArrayList;
import java.util.Scanner;

class Freq2 {
	public static void main(String[] args) {
		ArrayList<Integer> input = new ArrayList<Integer>();
		ArrayList<Integer> freq_history = new ArrayList<Integer>();
		
		Scanner s = new Scanner(System.in);
		while (s.hasNextInt()) {
			input.add(s.nextInt());
		}
		
		int freq = 0;
		while(true) {
			for(int i = 0; i < input.size(); ++i) {
				Integer in = input.get(i);
				freq += in;
				
				if(freq_history.contains(freq)) {
					System.out.println(freq);
					return;
				}
				
				freq_history.add(freq);
			}
		}
	}
}

