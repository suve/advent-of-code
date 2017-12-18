Program duet;

{$MODE OBJFPC}

Uses
	SysUtils;

Type
	TInstruction = (
		O_SND,
		O_SET,
		O_ADD,
		O_MUL,
		O_MOD,
		O_RCV,
		O_JGZ
	);
	
	TArgument = record
		Case IsRegister: Boolean of
			True: (Reg: Char);
			False: (Val: NativeInt);
	end;
	
	TOrder = record
		Instr: TInstruction;
		Arg1, Arg2: TArgument;
	end;

Var
	Reg:Array['a'..'z'] of NativeInt;
	PrevSnd: NativeInt;
	
	Order: Array of TOrder;

Function StrToInstruction(Const Name: AnsiString):TInstruction;
Begin
	Case Name of
		'snd': Result := O_SND;
		'set': Result := O_SET;
		'add': Result := O_ADD;
		'mul': Result := O_MUL;
		'mod': Result := O_MOD;
		'rcv': Result := O_RCV;
		'jgz': Result := O_JGZ;
		
		otherwise begin
			Writeln('watafuq? Unknown order "', Name, '"');
			Halt(1)
		end
	end;
End;

Function StrToArgument(Const str: AnsiString):TArgument;
Begin
	If(str[1] in ['a'..'z']) then begin
		Result.IsRegister := True;
		Result.Reg := str[1]
	end else begin
		Result.IsRegister := False;
		Result.Val := StrToInt(str)
	end
End;

Function ArgVal(Const Arg: TArgument):NativeInt;
Begin
	If(Arg.IsRegister) then
		Result := Reg[Arg.Reg]
	else
		Result := Arg.Val
End;

Procedure ReadInput();
Var
	Line: AnsiString;
	str_cmd, str_arg1, str_arg2: AnsiString;
	spc, Idx: NativeInt;
Begin
	While not eof() do begin
		Readln(Line);
		Writeln('> ', Line);
		
		str_cmd := Copy(Line, 1, 3);
		Delete(Line, 1, 4);
		
		spc := Pos(' ', Line);
		If(spc > 0) then begin
			str_arg1 := Copy(Line, 1, spc-1);
			Delete(Line, 1, spc);
			str_arg2 := Line
		end else begin
			str_arg1 := Line;
			str_arg2 := '0'
		end;
		
		Idx := Length(Order);
		SetLength(Order, Idx+1);
		
		Order[Idx].Instr := StrToInstruction(str_cmd); 
		Order[Idx].Arg1 := StrToArgument(str_arg1); 
		Order[Idx].Arg2 := StrToArgument(str_arg2); 
		
		// Writeln(' -> ', Order[Idx].Instr);
	end;
End;

Procedure ResetState();
Var
	R: Char;
Begin
	For R:='a' to 'z' do Reg[R] := 0;
	PrevSnd := 0
End;

Procedure Run();
Var
	Idx, Len: NativeInt;
Begin
	Idx := 0;
	Len := Length(Order);
	While((Idx >= 0) and (Idx < Len)) do begin
		Case Order[Idx].Instr of
			O_SND: PrevSnd := ArgVal(Order[Idx].Arg1);
			
			O_SET: Reg[Order[Idx].Arg1.Reg] := ArgVal(Order[Idx].Arg2);
			O_ADD: Reg[Order[Idx].Arg1.Reg] += ArgVal(Order[Idx].Arg2);
			O_MUL: Reg[Order[Idx].Arg1.Reg] *= ArgVal(Order[Idx].Arg2);
			O_MOD: Reg[Order[Idx].Arg1.Reg] := Reg[Order[Idx].Arg1.Reg] mod ArgVal(Order[Idx].Arg2);
			
			O_RCV: If(ArgVal(Order[Idx].Arg1) <> 0) then begin
				Writeln('Rcv: ', PrevSnd);
				Halt(1)
			end;
			
			O_JGZ: If(ArgVal(Order[Idx].Arg1) > 0) then Idx += ArgVal(Order[Idx].Arg2) - 1; 
		end;
		Idx += 1
	end;
End;

Begin
	ResetState();
	ReadInput();
	Run()
End.
