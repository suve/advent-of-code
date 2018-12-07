using System;
using System.Collections.Generic;

namespace AdventOfCode
{
	class Step
	{
		public char name;
		public int? timeUntilDone;
		public Dictionary<char, Step> requirements;
		public Dictionary<char, Step> descendants;
		
		public void AddRequirement(Step req)
		{
			Step entry;
			this.requirements.TryGetValue(req.name, out entry);
			if(entry == null) this.requirements[req.name] = req;
		}
		
		public void AddDescendant(Step desc)
		{
			Step entry;
			this.descendants.TryGetValue(desc.name, out entry);
			if(entry == null) this.descendants[desc.name] = desc;
		}
		
		public void RemoveRequirement(Step req)
		{
			this.requirements.Remove(req.name);
		}
		
		public void RemoveDescendant(Step desc)
		{
			this.descendants.Remove(desc.name);
		}
		
		public Step(char theName)
		{
			this.name = theName;
			this.requirements = new Dictionary<char, Step>();
			this.descendants = new Dictionary<char, Step>();
			this.timeUntilDone = null;
		}
	}
	
	class Day07 
	{
		private static Dictionary<char, Step> steps = new Dictionary<char, Step>();
		
		private static Step GetStep(char name)
		{
			Step step;
			Day07.steps.TryGetValue(name, out step);
			if(step == null)
			{
				step = new Step(name);
				Day07.steps[name] = step;
			}
			
			return step;
		}
		
		private static void ReadInput()
		{
			string line;
			while((line = Console.ReadLine()) != null)
			{
				string[] words = line.Split(" ".ToCharArray());
				if(words.Length < 8) continue;
				
				char nameR = words[1].ToCharArray()[0];
				char nameD = words[7].ToCharArray()[0];
				
				Step stepR = Day07.GetStep(nameR);
				Step stepD = Day07.GetStep(nameD);
				
				stepR.AddDescendant(stepD);
				stepD.AddRequirement(stepR);
			}
		}
		
		private static Step GetNextAvailableStep()
		{
			Step best = null;
			foreach(KeyValuePair<char, Step> entry in Day07.steps)
			{
				if(entry.Value.requirements.Count > 0) continue;
				if(entry.Value.timeUntilDone != null) continue;
				
				if(best != null)
				{
					if(best.name < entry.Value.name) continue;
				}
				
				best = entry.Value;
			}
			
			return best;
		}
		
		private static void AdvanceSteps()
		{
			foreach(KeyValuePair<char, Step> entry in Day07.steps)
			{
				if(entry.Value.timeUntilDone == null) continue;
				entry.Value.timeUntilDone -= 1;
			}
		}
		
		private static Step GetNextFinishedStep()
		{
			Step best = null;
			foreach(KeyValuePair<char, Step> entry in Day07.steps)
			{
				if(entry.Value.timeUntilDone != 0) continue;
				
				if(best != null)
				{
					if(best.name < entry.Value.name) continue;
				}
				
				best = entry.Value;
			}
			
			return best;
		}
		
		static void Main() 
		{
			Day07.steps = new Dictionary<char, Step>();
			Day07.ReadInput();
			
			int extraTime = 60;
			int workerLimit = 5;
			
			int simultaneousSteps = 0;
			int totalTime = 0;
			while(Day07.steps.Count > 0)
			{
				Step s;
				while( (simultaneousSteps < workerLimit) && ((s = Day07.GetNextAvailableStep()) != null) ) 
				{
					Console.Write(s.name);
					s.timeUntilDone = extraTime + (Convert.ToByte(s.name) - Convert.ToByte('A') + 1);
					simultaneousSteps += 1;
				}
				
				Day07.AdvanceSteps();
				
				while((s = Day07.GetNextFinishedStep()) != null)
				{
					Day07.steps.Remove(s.name);
					foreach(KeyValuePair<char, Step> entry in s.descendants)
					{
						entry.Value.RemoveRequirement(s);
					}
					
					simultaneousSteps -= 1;
				}
				
				totalTime += 1;
			}
			
			Console.WriteLine("");
			Console.WriteLine("Part 2 time: " + totalTime);
		}
	}
}
