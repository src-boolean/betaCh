#pragma once
#include <string>
#include <unordered_map>
#include <algorithm> 
#include <cctype> 
#include <locale>
#include <codecvt>


#include "../renderer/renderer.h"
#include "../interfaces/manager.h"

#include "../../sdk/globals.h"

class CFeatures {
public:
	class Misc {
	public:
		void Aimbot(C_UserCmd* cmd);
	};
	Misc m_Misc;
};
inline CFeatures* g_pFeatures = new CFeatures();
//class CFeatures {
//public:
//	class Aimbot {
//	public:
//		void Run(C_UserCmd* cmd);
//	};
//	Aimbot m_Aimbot;
//
//	class ESP {
//	public:
//		class Players {
//		public:
//			void Draw();
//			void Update();
//		private:
//			std::vector<std::pair<EntityListInfo, Vector3D>> m_EntitiesData;
//			void Box(CMath::BoundingBox bbox);
//			void Name(CMath::BoundingBox& pos, const EntityListInfo& player);
//			void HealthBar(CMath::BoundingBox& bbox, const EntityListInfo& player);
//			void Weapon(CMath::BoundingBox& boundingBox, const EntityListInfo& playerInfo);
//			void Ammo(CMath::BoundingBox& bbox, const EntityListInfo& pPawn);
//
//			void Distance(const EntityListInfo& playerInfo, CMath::BoundingBox& pos);
//
//			void Flags(const EntityListInfo& playerInfo, CMath::BoundingBox& pos);
//
//			void SnapLines(const EntityListInfo& playerInfo);
//
//			void Skeleton(const EntityListInfo& playerInfo);
//
//			std::unordered_map<int, float> previousHealth;
//
//			const int boneConnections[17][2] = {
//				{6, 5},  // head to neck
//				{5, 4},  // neck to spine
//				{4, 0},  // spine to hip
//				{4, 8},  // spine to left shoulder
//				{8, 9},  // left shoulder to left arm
//				{9, 11}, // arm to hand
//				{4, 13}, // spine to right shoulder
//				{13, 14},// right shoulder to arm
//				{14, 16},// right arm to hand
//				{4, 2},  // spine to spine_1
//				{0, 22}, // hip to left_hip
//				{0, 25}, // hip to right_hip
//				{22, 23},// left hip to left knee
//				{23, 24},// left knee to left foot
//				{25, 26},// right hip to right knee
//				{26, 27} // right knee to right foot
//			};
//		};
//		Players m_Players;
//
//		class World {
//		public:
//			void Draw();
//		private:
//			void Weapons(C_BaseEntity* Entity);
//			void Bomb(C_BaseEntity* Entity);
//			void Projectiles(C_BaseEntity* Entity);
//		};
//
//
//		class OthersVisuals {
//		public:
//			void Watermark();
//			void InfoPanel();
//			
//			void SniperScopeOverlay();
//		};
//		OthersVisuals m_OthersVisuals;
//
//		World m_World;
//
//		class PlayerList {
//		public:
//			void DrawPlayers();
//			int ClickedItem = -1;
//			void DrawInfoPanel();
//			void ClearPlayerData();
//			const char* Username = "";
//			uint64_t UserID = 0;
//			int Health = 0;
//		private:
//			std::vector<EntityListInfo> PlayerCacheList;
//		};
//	};
//	ESP m_ESP;
//};
//inline CFeatures* g_pFeatures = new CFeatures();


