import java.util.ArrayList;
import java.util.Scanner;

class Checksum {
	private static int twoCount;
	private static int threeCount;
	
	private static void CountDemChars(String str) {
		int[] count = new int[256];
		
		int len = str.length();
		for(int i = 0; i < len; ++i) {
			int c = Character.getNumericValue(str.charAt(i));
			++count[c];
		}
		
		boolean hasTwo = false;
		boolean hasThree = false;
		for(int i = Character.getNumericValue('a'); i <= Character.getNumericValue('z'); ++i) {
			if(count[i] == 2)
				hasTwo = true;
			else if(count[i] == 3)
				hasThree = true;
		}
		
		if(hasTwo) Checksum.twoCount += 1;
		if(hasThree) Checksum.threeCount += 1;
	}
	
	public static void main(String[] args) {
		Checksum.twoCount = 0;
		Checksum.threeCount = 0;
		
		Scanner s = new Scanner(System.in);
		while (s.hasNextLine()) {
			String str = s.nextLine();
			Checksum.CountDemChars(str);
		}
		
		System.out.println(Checksum.twoCount * Checksum.threeCount);
	}
}

