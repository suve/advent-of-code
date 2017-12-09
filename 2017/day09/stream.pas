Program day09; {$MODE OBJFPC} {$COPERATORS ON}

Var
	Stream: AnsiString;

Procedure ProcessGarbage(Idx: NativeInt; Out EndIdx, CharCount:NativeInt);
Begin
	Idx += 1; // Skip the opening '<'
	CharCount := 0;
	
	While (Stream[Idx] <> '>') do begin
		If(Stream[Idx] = '!') then begin
			Idx += 2
		end else begin
			CharCount += 1;
			Idx += 1
		end
	end;
	EndIdx := Idx
End;

Procedure ProcessGroup(Const StartIdx, Depth: NativeInt; Out EndIdx, TotalDepth, GarbageChars:NativeInt);
Var
	Idx, DepthSum, HowMuchGarbage: NativeInt;
	subgroup_end, subgroup_depth: NativeInt;
	garbage_end, garbage_count: NativeInt;
Begin
	DepthSum := Depth;
	HowMuchGarbage := 0;
	
	Idx := StartIdx+1;
	While (Stream[Idx] <> '}') do begin
		If(Stream[Idx] = '{') then begin
			ProcessGroup(Idx, Depth+1, subgroup_end, subgroup_depth, garbage_count);
			HowMuchGarbage += garbage_count;
			DepthSum += subgroup_depth;
			Idx := subgroup_end + 1
		end else If(Stream[Idx] = '<') then begin
			ProcessGarbage(Idx, garbage_end, garbage_count);
			HowMuchGarbage += garbage_count;
			Idx := garbage_end + 1 
		end else If(Stream[Idx] = ',') then begin
			Idx += 1
		end else begin
			Writeln(stderr, 'watafuq? unexpected character "', Stream[Idx], '"')
		end
	end;
	
	EndIdx := Idx;
	TotalDepth := DepthSum;
	GarbageChars := HowMuchGarbage
End;

Procedure ProcessInput();
Var
	Ignore, Depth, Garbage: NativeInt;
Begin
	ProcessGroup(1, 1, Ignore, Depth, Garbage);
	Writeln('-- depth: ', Depth, '; garbage: ', garbage)
End;

Begin
	While Not eof() do begin
		Readln(Stream);
		ProcessInput()
	end
End.
