Program day12;

Uses
	StrUtils, SysUtils;

Const
	MAX_PROGS = 2000;

Type
	TProg = record
		Group: NativeInt;
		Connection: Array of NativeInt
	end;

Var
	Prog: Array[0..MAX_PROGS-1] of TProg;
	GroupMembers: Array[0..MAX_PROGS-1] of NativeInt;
	ProgCount, GroupCount: NativeInt;
	Line: AnsiString;


Procedure ResetProgs();
Var
	Idx: NativeInt;
Begin
	ProgCount := 0;
	For Idx:=0 to (MAX_PROGS-1) do begin
		GroupMembers[Idx] := 0;
		
		Prog[Idx].Group := -1;
		SetLength(Prog[Idx].Connection, 0)
	end
End;

Procedure Connect(Const A, B: NativeInt);
Var
	L: NativeInt;
Begin
	L := Length(Prog[A].Connection);
	SetLength(Prog[A].Connection, L+1);
	Prog[A].Connection[L] := B
End;

Procedure ProcessLine();
Var
	ProgNo, ConnectTo: NativeInt;
	now, srch, len: NativeInt;
	part: AnsiString;
Begin
	srch := Pos('<', Line);
	part := Copy(Line, 1, srch-1);
	ProgNo := StrToInt(Trim(part));
	
	now := srch + 3;
	len := Length(Line);
	While(now <= len) do begin
		srch := PosEx(',', Line, now);
		If(srch > 0) then begin
			part := Copy(Line, now, srch-now);
			now := srch + 1
		end else begin
			part := Copy(Line, now, Len-now+1);
			now := Len+1
		end;
		
		ConnectTo := StrToInt(Trim(part));
		Connect(ProgNo, ConnectTo);
		Connect(ConnectTo, ProgNo)
	end
End;

Procedure AddNeighboursToGroup(Const ProgNo, TargetGroup: NativeInt);
Var
	Idx, Len, Nei: NativeInt;
Begin
	Len := Length(Prog[ProgNo].Connection);
	For Idx := 0 to (Len-1) do begin
		Nei := Prog[ProgNo].Connection[Idx];
		If(Prog[Nei].Group = -1) then begin
			Prog[Nei].Group := TargetGroup;
			GroupMembers[TargetGroup] += 1;
			
			AddNeighboursToGroup(Nei, TargetGroup)
		end
	end;
End;

Procedure CountGroups();
Var
	Idx: NativeInt;
Begin
	GroupCount := 0;
	For Idx := 0 to (ProgCount-1) do begin
		If(Prog[Idx].Group = -1) then begin
			GroupCount += 1;
			
			Prog[Idx].Group := Idx;
			GroupMembers[Idx] += 1;
			
			AddNeighboursToGroup(Idx, Idx)
		end
	end
End;

Begin
	ResetProgs();
	While Not eof() do begin
		Readln(Line);
		ProcessLine();
		ProgCount += 1
	end;
	
	CountGroups();
	Writeln('> Programs in p0 group: ', GroupMembers[Prog[0].Group]);
	Writeln('> Total groups: ', GroupCount)
End.
