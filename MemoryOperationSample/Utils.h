#ifndef _UTILS_H
#define _UTILS_H
#include "crappy.h"
#include "PMemHelper.h"

void UpdateAddresses();
void CacheNames();


DWORD_PTR decrypt(TSLEncryptDataObj encActor);
DWORD_PTR GetEntityList();
Vector3 GetLocalPlayerPos();
Vector3 GetActorPos(DWORD_PTR entity);
Vector3 GetBoneWithRotation(DWORD_PTR mesh, int id);
float GetActorHealth(DWORD_PTR pAActor);
int isUaz(int id);
int isDacia(int id);
int isBuggy(int id);
int isBike(int id);
int isBoat(int id);
int isAir(int id);
int isDeath(int id);
int isVan(int id);
int isPickup(int id);
void DrawSkeleton(DWORD_PTR mesh);
D3DMATRIX MatrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2);

void DrawVehicle(DWORD_PTR entity, Vector3 local, const char* txt);
Vector3 WorldToScreen(Vector3 WorldLocation, FCameraCacheEntry CameraCacheL);
FCameraCacheEntry GetCameraCache();
#endif