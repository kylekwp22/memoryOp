#define NOMINMAX

#include <limits>       // std::numeric_limits
#include<math.h>
#include "Utils.h"
#include <random>

std::random_device rd;
std::default_random_engine generator(rd()); // rd() provides a random seed
std::uniform_real_distribution<double> distribution(-1.5, 1.5);


//definitions
namespace global
{
	DWORD_PTR pUWorld = 0;
	DWORD_PTR pGameInstance = 0;
	DWORD_PTR pLocalPlayerArray = 0;
	DWORD_PTR pLocalPlayer = 0;
	DWORD_PTR pViewportClient = 0;
	bool bPlayer = true;
	bool bLoot = true;
	bool bVehicle = true;
	//bool bAim = false;
	bool bAimBot = false;
	bool ingame = false;
	FCameraCacheEntry cameracache = { NULL };

	bool speedUp = false;

	bool speedDown = false;

	float init_speed = 3.6;

}

void fix_zero(float dist) {

	auto playercontroller = mem->RPM<DWORD_PTR>(global::pLocalPlayer + 0x30, 8);
	auto Pawn = mem->RPM<DWORD_PTR>(playercontroller + 0x03B8, 8);
	auto pWeaponProcessor = mem->RPM<DWORD_PTR>(Pawn + 0x0A48, 8);
	int dwCurrentWeaponIndex = mem->RPM<int>(pWeaponProcessor + 0x04C8, 4);
	if (dwCurrentWeaponIndex >= 0 && dwCurrentWeaponIndex <= 2)
	{

		auto pEquippedWeapons = mem->RPM<DWORD_PTR>(pWeaponProcessor + 0x04B8, 8); //AtslWeapon
		auto pNowWeapon = mem->RPM<DWORD_PTR>(pEquippedWeapons + (dwCurrentWeaponIndex * 8), 8);//AtslWeapon_gun 0x0A20
		FWeaponData WeaponConfig = mem->RPM<FWeaponData>(pNowWeapon + 0x0550, 0x0090);

		WeaponConfig.CurrentWeaponZero = dist;


		WeaponConfig.SwayModifier_Crouch = 0;
		WeaponConfig.SwayModifier_Movement = 0;
		WeaponConfig.SwayModifier_Pitch = 0;
		WeaponConfig.SwayModifier_Prone = 0;
		WeaponConfig.SwayModifier_Stand = 0;
		WeaponConfig.SwayModifier_YawOffset = 0;
		mem->WPM<FWeaponData>(pNowWeapon + 0x0550, WeaponConfig, 0x0090);

	}

}

void speedUp() {
	auto playercontroller = mem->RPM<DWORD_PTR>(global::pLocalPlayer + 0x30, 8);
	auto pawn = mem->RPM<DWORD_PTR>(playercontroller + 0x0428, 8);
	auto stand_speed = mem->RPM<float>(pawn + 0x0E54, 0x4);
	//auto sprint_speed = mem->RPM<float>(pawn + 0x0E58, 0x4);
	//auto dilation = mem->RPM<float>(pawn + 0x0098, 0x4);
	if (global::speedUp)
	{ 

		if (pawn != NULL) {

			//mem->WPM<float>(pawn + 0x0E58, sprint_speed * 3.5f, 0x4);
			//mem->WPM<float>(pawn + 0x0098, 2.0f, 0x4);
			mem->WPM<float>(pawn + 0x0E54, global::init_speed*2.0f, 0x4);

		}
		
		
	}
	else {
		if (pawn != NULL) {
			//mem->WPM<float>(pawn + 0x0098, 1.0f, 0x4);
			mem->WPM<float>(pawn + 0x0E54, global::init_speed, 0x4);
			//mem->WPM<float>(pawn + 0x0E58, global::init_speed*1.7, 0x4);
		}
	}

	

}

void patch_aim()
{

	//wpm aimbot

	auto playercontroller = mem->RPM<DWORD_PTR>(global::pLocalPlayer + 0x30, 8);
	auto Pawn = mem->RPM<DWORD_PTR>(playercontroller + 0x03B8, 8);
	auto pWeaponProcessor = mem->RPM<DWORD_PTR>(Pawn + 0x0A48, 8);
	int dwCurrentWeaponIndex = mem->RPM<int>(pWeaponProcessor + 0x04C8, 4);
	if (dwCurrentWeaponIndex >= 0 && dwCurrentWeaponIndex <= 2)
	{
		auto pEquippedWeapons = mem->RPM<DWORD_PTR>(pWeaponProcessor + 0x04B8, 8); //AtslWeapon
		auto pNowWeapon = mem->RPM<DWORD_PTR>(pEquippedWeapons + (dwCurrentWeaponIndex * 8), 8);//AtslWeapon_gun 0x0A20


		FWeaponData WeaponConfig = mem->RPM<FWeaponData>(pNowWeapon + 0x0550, 0x0090);
		//auto weaponCurr = mem->RPM<DWORD_PTR>(pNowWeapon + 0x0180, 8); //ATslWeapon_Trajectory 0x0BA0

		FTrajectoryWeaponData TrajectoryConfig = mem->RPM<FTrajectoryWeaponData>(pNowWeapon + 0x0B18, 0x0080);
		FRecoilInfo RecoilInfo = mem->RPM<FRecoilInfo>(pNowWeapon + 0x0B98, 0x0068);
		FWeaponGunAnim WeaponGunAnim = mem->RPM<FWeaponGunAnim>(pNowWeapon + 0x0998, 0x00F0);
		//FWeaponDeviationData
		FWeaponDeviationData  WeaponDeviationConfig = mem->RPM<FWeaponDeviationData>(pNowWeapon + 0x0950, 0x0044);
		FWeaponGunData WeaponGunConfig = mem->RPM<FWeaponGunData>(pNowWeapon + 0x08C0, 0x0090);
		// TRAJECTORY CONFIG
		TrajectoryConfig.RecoilPatternScale = 0.1f;
		TrajectoryConfig.RecoilRecoverySpeed = 0;
		TrajectoryConfig.RecoilSpeed = 0;

		// RECOIL INFO
		RecoilInfo.VerticalRecoilMin = 0;
		RecoilInfo.VerticalRecoilMax = 0;
		RecoilInfo.RecoilValue_Climb = 0;
		RecoilInfo.RecoilValue_Fall = 0;
		RecoilInfo.RecoilModifier_Stand = 0;
		RecoilInfo.RecoilModifier_Crouch = 0;
		RecoilInfo.RecoilModifier_Prone = 0;
		RecoilInfo.RecoilSpeed_Horizontal = 0;
		RecoilInfo.RecoilSpeed_Vertical = 0;
		RecoilInfo.RecoverySpeed_Vertical = 0;
		RecoilInfo.VerticalRecoveryModifier = 0;
		WeaponGunAnim.ShotCameraShake = NULL;
		WeaponGunAnim.ShotCameraShakeADS = NULL;
		WeaponGunAnim.ShotCameraShakeIronsight = NULL;
		WeaponGunAnim.RecoilKickADS = 0;

		

		//NoSpread
		TrajectoryConfig.WeaponSpread = 0;
		TrajectoryConfig.AimingSpreadModifier = 0;
		TrajectoryConfig.FiringSpreadBase = 0;
		TrajectoryConfig.ProneRecoveryTime = 0;
		TrajectoryConfig.ScopingSpreadModifier = 0;


		TrajectoryConfig.VDragCoefficient = 0.0001f;
		TrajectoryConfig.BallisticCurve = NULL;

		

		WeaponGunConfig.FiringBulletsSpread = 0;

		WeaponDeviationConfig.DeviationBase = 0;
		WeaponDeviationConfig.DeviationBaseADS = 0;
		WeaponDeviationConfig.DeviationBaseAim = 0;
		WeaponDeviationConfig.DeviationMax = 0;
		WeaponDeviationConfig.DeviationMaxMove = 0;
		WeaponDeviationConfig.DeviationMinMove = 0;
		WeaponDeviationConfig.DeviationMoveMaxReferenceVelocity = 0;
		WeaponDeviationConfig.DeviationMoveMinReferenceVelocity = 0;
		WeaponDeviationConfig.DeviationMoveMultiplier = 0;
		WeaponDeviationConfig.DeviationRecoilGain = 0;
		WeaponDeviationConfig.DeviationRecoilGainADS = 0;
		WeaponDeviationConfig.DeviationRecoilGainAim = 0;
		WeaponDeviationConfig.DeviationStanceCrouch = 0;
		WeaponDeviationConfig.DeviationStanceJump = 0;
		WeaponDeviationConfig.DeviationStanceProne = 0;
		WeaponDeviationConfig.DeviationStanceStand = 0;

		//magic bullets
		//WeaponGunConfig.TimeBetweenShots = 0.03f;
		//no muzzle
		WeaponGunAnim.CharacterFire = NULL;

		//instant hit
		
		//TrajectoryConfig.TravelDistanceMax = 80000.0f;
		TrajectoryConfig.InitialSpeed = 20000.00f;
		if(pNowWeapon!=NULL)
		{
		
			mem->WPM<float>(pNowWeapon + 0x0AF0, 0, 0x0004); // gravity

			mem->WPM<float>(pNowWeapon + 0x0AF4, 0, 0x0004); //RecoilSpreadScale

			mem->WPM<float>(pNowWeapon + 0x0B08, 0, 0x0004); //walkSpread
			mem->WPM<float>(pNowWeapon + 0x0B0C, 0, 0x0004); //Jump Spread
			mem->WPM<float>(pNowWeapon + 0x0B10, 0, 0x0004); //Run Spread
		
		
		}
		
		//no sway <-Possible Corruption
		/**/
		WeaponConfig.SwayModifier_Crouch = 0;
		WeaponConfig.SwayModifier_Movement = 0;
		WeaponConfig.SwayModifier_Pitch = 0;
		WeaponConfig.SwayModifier_Prone = 0;
		WeaponConfig.SwayModifier_Stand = 0;
		WeaponConfig.SwayModifier_YawOffset = 0;
		
		

		
		//if (pNowWeapon != NULL)
		//{
			mem->WPM<FWeaponGunAnim>(pNowWeapon + 0x0998, WeaponGunAnim, 0x00F0);
			mem->WPM<FTrajectoryWeaponData>(pNowWeapon + 0x0B18, TrajectoryConfig, 0x0080);
			mem->WPM<FRecoilInfo>(pNowWeapon + 0x0B98, RecoilInfo, 0x0068);

			mem->WPM<FWeaponData>(pNowWeapon + 0x0550, WeaponConfig, 0x0090);


			mem->WPM<FWeaponDeviationData>(pNowWeapon + 0x0950, WeaponDeviationConfig, 0x0044);
			mem->WPM<FWeaponGunData>(pNowWeapon + 0x08C0, WeaponGunConfig, 0x0090);
		
		//}
		

		

	}
}//no recoil finished




void ESP()
{
	if (GetAsyncKeyState(VK_CAPITAL) & 1)
		global::bAimBot = global::bAimBot ? false : true;


	if (GetAsyncKeyState('6') & 1)
		global::speedUp = global::speedUp? false : true;


	//check in game
	/*auto playercontroller = mem->RPM<DWORD_PTR>(global::pLocalPlayer + 0x30, 8);
		auto hud = mem->RPM<DWORD_PTR>(playercontroller + 0x430, 8);
		int playercontroller_id = mem->RPM<int>(playercontroller + 0x18, 0x4);
		int hud_id = mem->RPM<int>(hud + 0x18, 0x4); */
	
	UpdateAddresses();
	wchar_t entityname[64] = { NULL };
	ZeroMemory(entityname, sizeof(entityname));
	DWORD_PTR enlist = GetEntityList();
	int entitycount = mem->RPM<int>(mem->RPM<DWORD_PTR>(global::pUWorld + 0x30, 0x8) + 0xB8, 0x4);
	float health = 0.f;
	float distance = 0.f;
	float item_distance = 0.f;

	DWORD color = 0;
	Vector3 local = GetLocalPlayerPos();

	
	
	

	for (int i = 0; i < entitycount; i++)
	{
		
		/*auto entity = mem->RPM<TSLEncryptDataObj>(enlist + (i * 0x168), 0x168);
		TSLEncryptDataObj temp_obj;
		uint8_t* readLoc = (uint8_t*)((&temp_obj));

		for (int p = 0; p < sizeof(TSLEncryptDataObj); p += 8) {
			mem->ReadVirtual(cr3, enlist + (i * sizeof(TSLEncryptDataObj)) + p, readLoc, 8);
			readLoc += 0x8;
		}


		auto decActor = decrypt(temp_obj);
		if (!decActor)
			continue;
			*/
		auto decActor = mem->RPM<DWORD_PTR>(enlist + (i * 0x8), 0x8);
		if (!decActor)
			continue;
		
		
	

		int id = mem->RPM<int>(decActor + 0x18, 0x4);
		// 
		if (global::bPlayer && (id == ActorIds[0] || id == ActorIds[1] || id == ActorIds[2] || id == ActorIds[3]))
		//if (global::bPlayer)
		{
			
			//health = GetActorHealth(entity);
			//if (health > 0.f)
			//{
			
				//auto aPlayerState = mem->RPM<DWORD_PTR>(decActor + 0x03D0, 8);
				//FString Name = mem->RPM<FString>(aPlayerState + 0x03B8, sizeof(FString));
				
			
				//0x0D68 char name;

				//FString Name = mem->RPM<FString>(decActor + 0x0D68, sizeof(FString));


				Vector3 pos = GetActorPos(decActor);
				Vector3 spos = WorldToScreen(pos, global::cameracache);
				distance = local.Distance(pos) / 100.f;

				if (distance > 850.f)
					continue;
				if (distance <= 150.f)
					color = D3DCOLOR_ARGB(255, 255, 0, 0); //color red, if less than 150m
				else if (distance > 150.f && distance <= 300.f)
					color = D3DCOLOR_ARGB(255, 255, 255, 0); //color yellow, if less than 300m and greater than 150m
				else
					color = D3DCOLOR_ARGB(255, 0, 255, 0); //color green, if greater than 300m

				DrawString((int)spos.x, (int)spos.y, color, pFont, "[%0.2f]", distance);

				//DrawString((int)spos.x, (int)spos.y, color, pFont, "[%ws %0.2f]", Name.c_str(),distance);
				/*	auto mesh = mem->RPM<DWORD_PTR>(decActor + 0x410, 0x8);
				if (!mesh && distance > 110.f)
					continue;
				DrawSkeleton(mesh);*/
			
				
			//}
		}


		//vehicle esp
		if (global::bVehicle)
		{
			if (isUaz(id))
				DrawVehicle(decActor, local, "UAZ\nDistance: %0.2f");
			else if (isDacia(id))
				DrawVehicle(decActor, local, "Dacia\nDistance: %0.2f");
			else if (isBike(id))
				DrawVehicle(decActor, local, "Motorbike\nDistance: %0.2f");
			else if (isBuggy(id))
				DrawVehicle(decActor, local, "Buggy\nDistance: %0.2f");
			else if (isBoat(id))
				DrawVehicle(decActor, local, "Boat\nDistance: %0.2f");
			else if (isVan(id))
				DrawVehicle(decActor, local, "Van\nDistance: %0.2f");
			else if (isPickup(id))
				DrawVehicle(decActor, local, "Pickup\nDistance: %0.2f");


		}

		if (isAir(id)) 
		{
			DrawVehicle(decActor, local, "AirDrop\nDistance: %0.2f");
		}
		else if (isDeath(id))
		{
			DrawVehicle(decActor, local, "DeathDrop\nDistance: %0.2f");
		}

		if (global::bLoot && (id == itemtype[0] || id == itemtype[1]) )
		{
			Vector3 pos = GetActorPos(decActor);
			Vector3 spos = WorldToScreen(pos, global::cameracache);
			item_distance = local.Distance(pos) / 100.f;

			DWORD_PTR DroppedItemGroupArray = mem->RPM<DWORD_PTR>(decActor + 0x2E8, 0x8);
			int count = mem->RPM<int>(decActor + 0x2F0, 0x4);
			
			if (!count)
				continue;
			for (int j = 0; j < count; j++)
			{
				DWORD_PTR pADroppedItemGroup = mem->RPM<DWORD_PTR>(DroppedItemGroupArray + j * 0x10, 0x8);
				Vector3 relative = mem->RPM<Vector3>(pADroppedItemGroup + 0x2D0, 0xC);
				Vector3 screenloc = WorldToScreen(GetActorPos(decActor) + relative, global::cameracache);
				DWORD_PTR pUItem = mem->RPM<DWORD_PTR>(pADroppedItemGroup + 0x538, 0x8);
				DWORD_PTR pUItemFString = mem->RPM<DWORD_PTR>(pUItem + 0x40, 0x8);
				DWORD_PTR pItemName = mem->RPM<DWORD_PTR>(pUItemFString + 0x28, 0x8);

				ZeroMemory(entityname, sizeof(entityname));
				if (mem->RPMWSTR(pItemName, entityname, sizeof(entityname))) 
				{
					if (wcsstr(entityname, L"M416") != 0 || wcsstr(entityname, L"SCAR") != 0 || wcsstr(entityname, L"98") != 0
						|| wcsstr(entityname, L"M16A4") != 0 || (wcsstr(entityname, L"Compensator") != 0 && wcsstr(entityname, L"AR") != 0) || (wcsstr(entityname, L"Suppressor") != 0 && wcsstr(entityname, L"AR") != 0)
						|| wcsstr(entityname, L"Scope") != 0 || wcsstr(entityname, L"5.56") != 0 || wcsstr(entityname, L"Dot") != 0 
						|| wcsstr(entityname, L"First") != 0 || wcsstr(entityname, L"Backpack") != 0 || wcsstr(entityname, L"AKM") != 0 || wcsstr(entityname, L"7.62") != 0
						|| (wcsstr(entityname, L"Suppressor") != 0 && wcsstr(entityname, L"SR") != 0) || wcsstr(entityname, L"SKS") != 0 || wcsstr(entityname, L"Drink") != 0
						|| wcsstr(entityname, L"Pain") != 0 || wcsstr(entityname, L"Helmet") != 0 || wcsstr(entityname, L"Vest") != 0
						|| wcsstr(entityname, L"AUG") != 0)
					{
						if (item_distance <= 500.f)
						{
							if ((wcsstr(entityname, L"Suppressor") != 0 && wcsstr(entityname, L"AR") != 0)
								|| (wcsstr(entityname, L"Suppressor") != 0 && wcsstr(entityname, L"SR") != 0) ||
								wcsstr(entityname, L"Lv.3") != 0 || wcsstr(entityname, L"4x") != 0 || wcsstr(entityname, L"8x") ||
									wcsstr(entityname, L"AKM") != 0 || wcsstr(entityname, L"M416") != 0 || wcsstr(entityname, L"98") != 0
								|| wcsstr(entityname, L"Level 3") != 0

								)
							{
								DrawString(screenloc.x, screenloc.y, D3DCOLOR_ARGB(255, 80, 66, 244), pFont, "%ws", entityname);

							}
							else
							{
								DrawString(screenloc.x, screenloc.y, D3DCOLOR_ARGB(255, 255, 144, 0), pFont, "%ws", entityname);
							}
						}
					}
				}

			}
		}


	}//first entity loop ends here


	patch_aim();

	
	//speedUp();


	if (global::bAimBot)
	{
		auto playercontroller = mem->RPM<DWORD_PTR>(global::pLocalPlayer + 0x30, 8);
		auto vecLocalEyeLocation = GetCameraCache().POV.Location;
		auto angLocalAngles = mem->RPM<FRotator>(playercontroller + 0x03E0, 0x000C);
		
		float flFov = 90;
		FRotator flBestAngDelta = FRotator();

		bool bFoundTarget = false;
		DWORD_PTR enlist = GetEntityList();
		int entitycount = mem->RPM<int>(mem->RPM<DWORD_PTR>(global::pUWorld + 0x30, 0x8) + 0xB8, 0x4);
		
		for (int i = 0; i < entitycount; i++)
		{
		
			//auto entity = mem->RPM<TSLEncryptDataObj>(enlist + (i * 0x168), 0x168);
			/*	TSLEncryptDataObj temp_obj;
			uint8_t* readLoc = (uint8_t*)((&temp_obj));

			for (int p = 0; p < sizeof(TSLEncryptDataObj); p += 8) {
				mem->ReadVirtual(cr3, enlist + (i * sizeof(TSLEncryptDataObj)) + p, readLoc, 8);
				readLoc += 0x8;
			}


			auto decActor = decrypt(temp_obj);

			
			if (!decActor)
				continue;*/
		

			auto decActor = mem->RPM<DWORD_PTR>(enlist + (i * 0x8), 0x8); //AtslCharacter
			if (!decActor)
				continue;


			DWORD_PTR teamObj = mem->RPM<DWORD_PTR>(decActor + 0x0CF8, 0x8);
			if (teamObj != NULL)
				continue;

			
			health = GetActorHealth(decActor);
			if (health <= 0.f)
				continue;
			
			

			int id = mem->RPM<int>(decActor + 0x18, 0x4);
			
			if ((id == ActorIds[0] || id == ActorIds[1] || id == ActorIds[2] || id == ActorIds[3]))
			{
			
				auto mesh = mem->RPM<DWORD_PTR>(decActor + 0x410, 0x8);
				
				auto vecTargetCenterOfMass = GetBoneWithRotation(mesh, Bones::forehead);

				

				if ((local.Distance(vecTargetCenterOfMass) / 100.f) > 750.f)
					continue;

				if ((local.Distance(vecTargetCenterOfMass) / 100.f) > 660.f)
				{vecTargetCenterOfMass = GetBoneWithRotation(mesh, Bones::spine_02);
					//vecTargetCenterOfMass = GetActorPos(entity);
				}

				fix_zero(local.Distance(vecTargetCenterOfMass) / 100.f);
				
				double number = distribution(generator);
				vecTargetCenterOfMass.z += number;
					
				auto delta = vecTargetCenterOfMass - vecLocalEyeLocation;
				auto angDelta = (delta.ToFRotator() - angLocalAngles).Clamp();

				

				if (angDelta.Length() <= flFov)
				{

					
					flFov = (float)angDelta.Length();
					flBestAngDelta = angDelta;
					bFoundTarget = true;
				}
			}


			}
		//cout << bFoundTarget << endl;;
		// TODO: Keyboard Hook
		if (bFoundTarget) { // ADD HOLD KEY OR WHATEVER

			mem->WPM<FRotator>(playercontroller + 0x03E0, (angLocalAngles + flBestAngDelta), 0x000C);

		}
	}//aimbot ends here

}




//render function
void render()
{
	// clear the window alpha
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	d3ddev->BeginScene();    // begins the 3D scene

							 //calculate and and draw esp stuff
	ESP();


	d3ddev->EndScene();    // ends the 3D scene

	d3ddev->Present(NULL, NULL, NULL, NULL);   // displays the created frame on the screen
}

//set up overlay window
void SetupWindow()
{

	RECT rc;
	WNDCLASSEX wc;

	//DWORD my_pid;

	while (!twnd)
		twnd = FindWindow(L"UnrealWindow", 0);
	if (twnd != NULL)
	{
		GetWindowRect(twnd, &rc);
		s_width = rc.right - rc.left;
		s_height = rc.bottom - rc.top;
	}
	

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandle(0);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)RGB(0, 0, 0);
	wc.lpszClassName = L"kwanga";
	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TRANSPARENT, wc.lpszClassName, L"KKKK", WS_POPUP, rc.left, rc.top, s_width, s_height, NULL, NULL, wc.hInstance, NULL);

	SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 0, ULW_COLORKEY);
	SetLayeredWindowAttributes(hWnd, 0, 255, LWA_ALPHA);

	ShowWindow(hWnd, SW_SHOW);
	initD3D(hWnd);
	cout << "window setup finished" << endl;
}

WPARAM MainLoop()
{
	MSG msg;
	RECT rc;

	while (TRUE)
	{
		ZeroMemory(&msg, sizeof(MSG));
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
			exit(0);
		twnd = NULL;
		twnd = FindWindow(L"UnrealWindow", 0);
		if (!twnd)
		{
			ExitProcess(0);
		}
		ZeroMemory(&rc, sizeof(RECT));
		GetWindowRect(twnd, &rc);
		s_width = rc.right - rc.left;
		s_height = rc.bottom - rc.top;
		MoveWindow(hWnd, rc.left, rc.top, s_width, s_height, true);
	
		
		render();


		HWND hwnd2 = GetForegroundWindow();
		HWND hwnd3 = GetWindow(hwnd2, GW_HWNDPREV);
		SetWindowPos(hWnd, hwnd3, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		UpdateWindow(hWnd);

		//Sleep(1);
	}
	return msg.wParam;
}

void main()
{

	
	mem = new PMemHelper();



	SetupWindow();
	CacheNames();
	uint32_t ret = (uint32_t)MainLoop();
	cin.get();
	
	return ;

}