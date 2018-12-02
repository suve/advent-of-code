import java.util.ArrayList;
import java.util.Scanner;

class FindIDs {
	public static void main(String[] args) {
		ArrayList<String> input = new ArrayList<String>();
		
		Scanner s = new Scanner(System.in);
		while (s.hasNextLine()) {
			input.add(s.nextLine());
		}
		
		int count = input.size();
		for(int i = 0; i < count; ++i) {
			for(int j = i+1; j < count; ++j) {
				String si = input.get(i);
				String sj = input.get(j);
				
				int len = si.length();
				if(len != sj.length()) continue;
				
				int difference = 0;
				for(int c = 0; c < len; ++c) {
					if(si.charAt(c) != sj.charAt(c)) {
						++difference;
						if(difference > 1) break;
					}
				}
				
				if(difference != 1) continue;
				
				System.out.println(si);
				System.out.println(sj);
				
				for(int c = 0; c < len; ++c) {
					if(si.charAt(c) == sj.charAt(c)) System.out.print(si.charAt(c));
				}
				System.out.println("");
				
				return;
			}
		}
	}
}

