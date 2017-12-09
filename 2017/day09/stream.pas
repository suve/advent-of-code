Program day09; {$MODE OBJFPC} {$COPERATORS ON}

Var
	Stream: AnsiString;

Function ProcessGarbage(Idx: NativeInt):NativeInt;
Begin
	While (Stream[Idx] <> '>') do begin
		If(Stream[Idx] = '!') then
			Idx += 2
		else
			Idx += 1
	end;
	Result := Idx
End;

Procedure ProcessGroup(Const StartIdx, Depth: NativeInt; Out EndIdx, TotalDepth:NativeInt);
Var
	Idx, DepthSum: NativeInt;
	subgroup_end, subgroup_depth, garbage_end: NativeInt;
Begin
	DepthSum := Depth;
	
	Idx := StartIdx+1;
	While (Stream[Idx] <> '}') do begin
		If(Stream[Idx] = '{') then begin
			ProcessGroup(Idx, Depth+1, subgroup_end, subgroup_depth);
			DepthSum += subgroup_depth;
			Idx := subgroup_end + 1
		end else If(Stream[Idx] = '<') then begin
			garbage_end := ProcessGarbage(Idx);
			Idx := garbage_end + 1 
		end else If(Stream[Idx] = ',') then begin
			Idx += 1
		end else begin
			Writeln(stderr, 'watafuq? unexpected character "', Stream[Idx], '"')
		end
	end;
	
	EndIdx := Idx;
	TotalDepth := DepthSum
End;

Function ProcessInput():NativeInt;
Var
	Ignore, Depth: NativeInt;
Begin
	ProcessGroup(1, 1, Ignore, Depth);
	Result := Depth
End;

Begin
	While Not eof() do begin
		Readln(Stream);
		Writeln('-- ', ProcessInput)
	end
End.
