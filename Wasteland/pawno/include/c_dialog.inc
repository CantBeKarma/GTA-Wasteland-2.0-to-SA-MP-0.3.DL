/*  Callback Dialogs
 *
 *  (c) Copyright 2014, Dimazzz
 *
 */

#if defined _cdialog_included
	#endinput
#endif
#define _cdialog_included
#pragma library cdialog

#include <memory>

#define CALLBACK_DIALOG_ID 	32767 	// ID ������� � ������� ����� ��������.
#define MAX_CALLBACK_NAME 	32 	// ������������ ����� ����� �������.

#define Dialog:%1(%2)          			\
			forward %1(%2); 		\
			public %1(%2)
#if !defined isnull
	#define isnull(%1)	((!(%1[0])) || (((%1[0]) == '\1') && (!(%1[1]))))
#endif

Dialog_ShowCallback(playerid, callback[], style, name[], text[], button1[], button2[]) 
{
	new 
		Pointer:mem = MEM::malloc(MAX_CALLBACK_NAME) // �������� ������
	; 
	if(_:mem != NULL) 
	{
		MEM::set_arr(mem, _, callback, MAX_CALLBACK_NAME); // ������� �������� ������� � ������
		SetPVarInt(playerid, "cdialogmem", _:mem); // ������� ������ ������ ������ � PVar
	}
	else
	{
		print("[ERROR]: Failed to allocate memory for \"c_Dialog\"!");
		return 0;
	}
	SetPVarString(playerid, "lastDialog", callback);
	ShowPlayerDialog(playerid, CALLBACK_DIALOG_ID, style, name, text, button1, button2); // ���������� ���������� ����
	return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if(dialogid == CALLBACK_DIALOG_ID && GetPVarType(playerid, "cdialogmem"))
	{
		new 
			Pointer:mem = Pointer:GetPVarInt(playerid, "cdialogmem"), // �������� ������ ������
			callback[MAX_CALLBACK_NAME]
		;
		MEM::get_arr(mem, _, callback); // ����� �������� ������� �� ������
		MEM::free(mem); // ������� ������
		DeletePVar(playerid, "cdialogmem"); // ������� PVar
		if(isnull(inputtext)) CallLocalFunction(callback,"iiiis", playerid, dialogid, response, listitem, "\1"); // �������� ������
		else CallLocalFunction(callback,"iiiis", playerid, dialogid, response, listitem, inputtext); // �������� ������
		return 1; 
	}
	else if(GetPVarType(playerid, "cdialogmem")) // ���� �� ������� ������, � � ������ ���� ������ ������� ���
	{
		#if defined SECURE_MEMORY_PLUGIN // ���� ������� ���������� ����� memory access plugin'a
			if(MEM::is(Pointer:GetPVarInt(playerid, "cdialogmem"))) 
			{
				MEM::free(Pointer:GetPVarInt(playerid, "cdialogmem")); // ������� ������
			}
		#else
			MEM::free(Pointer:GetPVarInt(playerid, "cdialogmem")); // ������� ������
		#endif
		DeletePVar(playerid, "cdialogmem"); // ������� PVar
	}
	
	
	#if defined cDialog_OnDialogResponse
		return cDialog_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
	#else
		return 1;
	#endif
}

public OnPlayerDisconnect(playerid, reason)
{
	if(GetPVarType(playerid, "cdialogmem")) // ���� ������ ������� � PVar.
	{
		#if defined SECURE_MEMORY_PLUGIN // ���� ������� ���������� ����� memory access plugin'a
			if(MEM::is(Pointer:GetPVarInt(playerid, "cdialogmem"))) 
			{
				MEM::free(Pointer:GetPVarInt(playerid, "cdialogmem")); // ������� ������
			}
		#else
			MEM::free(Pointer:GetPVarInt(playerid, "cdialogmem")); // ������� ������
		#endif
		DeletePVar(playerid, "cdialogmem"); // ������� PVar
	}

	#if defined cDialog_OnPlayerDisconnect
		return cDialog_OnPlayerDisconnect(playerid, reason);
	#else
		return 1;
	#endif
}



// Hooks
#if defined _ALS_OnDialogResponse
	#undef OnDialogResponse
#else
	#define _ALS_OnDialogResponse
#endif
#define OnDialogResponse cDialog_OnDialogResponse
#if defined cDialog_OnDialogResponse
	forward cDialog_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
#endif

#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect cDialog_OnPlayerDisconnect
#if defined cDialog_OnPlayerDisconnect
	forward OnPlayerDisconnect(playerid, reason);
#endif