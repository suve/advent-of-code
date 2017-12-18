Program duet2;

{$MODE OBJFPC}

Uses
	CThreads, SysUtils, Classes, BaseUnix;

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
	
	TThreadState = (
		STATE_INIT,
		STATE_RUNNING,
		STATE_WAITING,
		STATE_TERMINATED
	); 
	
	TDuetThread = class(TThread)
		Private
			ID: NativeInt;
			Reg: Array['a'..'z'] of NativeInt;
			SndFile, RcvFile: Text;
		
			Function ArgVal(Const Arg: TArgument):NativeInt;
		
		Protected
			Procedure Execute(); Override;
		
		Public
			State: TThreadState;
			SndCounter: NativeInt;
			
			Constructor Create(Const withID: NativeInt);
	end;

Var
	Thr: Array[0..1] of TDuetThread;
	Order: Array of TOrder;
	
	Sleepy: NativeInt;

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


Constructor TDuetThread.Create(Const withID:NativeInt);
Var
	R: Char;
Begin
	Inherited Create(True);
	
	Self.ID := withID;
	Self.State := STATE_INIT;
	Self.SndCounter := 0;
	
	Assign(Self.SndFile, 'queue' + IntToStr((ID + 1) mod 2) + '.fifo');
	Rewrite(Self.SndFile); 
	Writeln('Thread ', Self.ID, ': Rewrite()');
	
	For R:='a' to 'z' do Self.Reg[R] := 0;
	Self.Reg['p'] := withID;
	
	Writeln('Thread ', Self.ID, ' created');
End;

Function TDuetThread.ArgVal(Const Arg: TArgument):NativeInt;
Begin
	If(Arg.IsRegister) then
		Result := Self.Reg[Arg.Reg]
	else
		Result := Arg.Val
End;

Procedure TDuetThread.Execute();
Var
	Idx, Len: NativeInt;
	Received: AnsiString;
Begin
	Assign(Self.RcvFile, 'queue' + IntToStr(ID) + '.fifo');
	Reset(Self.RcvFile); 
	Writeln('Thread ', Self.ID, ': Reset()');
	
	Self.State := STATE_RUNNING;
	Writeln('Thread ', Self.ID, ' started');
	
	Idx := 0;
	Len := Length(Order);
	While((Idx >= 0) and (Idx < Len)) do begin
		Case Order[Idx].Instr of
			O_SND: begin
				Writeln(Self.SndFile, ArgVal(Order[Idx].Arg1));
				Flush(Self.SndFile);
				Self.SndCounter += 1;
				Writeln('Thread ', Self.ID, ' sent ', ArgVal(Order[Idx].Arg1));
			end;
			
			O_RCV: begin
				Self.State := STATE_WAITING;
				Repeat
					Readln(Self.RcvFile, Received);
					Sleep(10);
				Until (Received <> '');
				Reg[Order[Idx].Arg1.Reg] := StrToInt(Received);
				Self.State := STATE_RUNNING;
				
				Writeln('Thread ', Self.ID, ' received "', Received, '"');
			end;
			
			O_SET: Reg[Order[Idx].Arg1.Reg] := ArgVal(Order[Idx].Arg2);
			O_ADD: Reg[Order[Idx].Arg1.Reg] += ArgVal(Order[Idx].Arg2);
			O_MUL: Reg[Order[Idx].Arg1.Reg] *= ArgVal(Order[Idx].Arg2);
			O_MOD: Reg[Order[Idx].Arg1.Reg] := Reg[Order[Idx].Arg1.Reg] mod ArgVal(Order[Idx].Arg2);
			
			O_JGZ: If(ArgVal(Order[Idx].Arg1) > 0) then Idx += ArgVal(Order[Idx].Arg2) - 1; 
		end;
		Idx += 1
	end;
	Self.State := STATE_TERMINATED
End;

Begin
	ReadInput();
	
	// FPmkfifo('queue0.fifo', ((((6) * 8) + 6) * 8) + 0);
	// FPmkfifo('queue1.fifo', ((((6) * 8) + 6) * 8) + 0);
	
	Thr[0] := TDuetThread.Create(0);
	Thr[1] := TDuetThread.Create(1);
	
	Thr[0].Start();
	Thr[1].Start();
	
	Sleepy := 0;
	While True do begin
		Sleep(5);
		If(Thr[0].State = STATE_TERMINATED) then begin
			Writeln('> prog0 terminated');
			Break
		end else
		If(Thr[1].State = STATE_TERMINATED) then begin
			Writeln('> prog1 terminated');
			Break
		end else
		If(Thr[0].State = STATE_WAITING) AND (Thr[1].State = STATE_WAITING) then begin
			Sleepy += 1;
			If(Sleepy = 1000) then begin
				Writeln('> possible deadlock');
				Break;
			end
		end else begin
			// Writeln('sleep reset');
			Sleepy := 0
		end
	end;
	
	Writeln('> prog0 send: ', Thr[0].SndCounter);
	Writeln('> prog1 send: ', Thr[1].SndCounter);
	Halt(0)
End.
