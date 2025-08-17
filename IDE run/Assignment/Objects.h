#pragma once
typedef struct position {
	int X;
	int Y;
}coord; 
class Object { // Everything
	public:
		char T;
		coord p;
};

//----------------AVATAR---------------------------
class Avatar : public Object { // Subclass for avatar
	int potioncount;
	char Team;
	public:
		void SetPotionCount(int Potion) {
			potioncount = Potion;
		}
		int GetPotionCount() {
			return potioncount;
		}
		void SetTeam(char Tm) {
			Team = Tm;
		}
		int GetTeam() {
			return Team;
		}
		Avatar(int pt, char tm, char ty) {
			potioncount = pt;
			Team = tm;
			T = ty;
		}

};

/*-----------------------ENTITIES--------------------------------*/
class Werewolf : public Object { // Subclass for Werewolves
	int HP, ATK, DEF, MED, HF, CF;
	public:
		void SetHP(int hp) {
			HP = hp;
		}
		int GetHP() {
			return HP;
		}

		void SetDEF(int def) {
			DEF = def;
		}
		int GetDEF() {
			return DEF;
		}

		void SetATK(int atk) {
			ATK = atk;
		}
		int GetATK() {
			return ATK;
		}
		void SetMED(int med) {
			MED = med;
		}
		int GetMED() {
			return MED;
		}
		void SetHF(int flag) {
			HF = flag;
		}
		int GetHF() {
			return HF;
		}
		void SetCF(int flag) {
			CF = flag;
		}
		int GetCF() {
			return CF;
		}
	Werewolf(int H, int A, int D, int M, char ty, int flagH, int flagC) {
		HP = H;
		ATK = A;
		DEF = D;
		MED = M; 
		T = ty;
		HF = flagH;
		CF = flagC;
	}
};

class Vampire : public Object { // Subclass for Werewolves
	int HP, ATK, DEF, MED, HF, CF;
public:
	void SetHP(int hp) {
		HP = hp;
	}
	int GetHP() {
		return HP;
	}

	void SetDEF(int def) {
		DEF = def;
	}
	int GetDEF() {
		return DEF;
	}

	void SetATK(int atk) {
		ATK = atk;
	}
	int GetATK() {
		return ATK;
	}
	void SetMED(int med) {
		MED = med;
	}
	int GetMED() {
		return MED;
	}
	void SetHF(int flag) {
		HF = flag;
	}
	int GetHF() {
		return HF;
	}
	void SetCF(int flag) {
		CF = flag;
	}
	int GetCF() {
		return CF;
	}
	Vampire(int H, int A, int D, int M, char ty, int flagH, int flagC) {
		HP = H;
		ATK = A;
		DEF = D;
		MED = M;
		T = ty;
		HF = flagH;
		CF = flagC;
	}
};

//-----------------OBSTACLES----------------------
class Obstacle: public Object { // Tree / Lake
	char Type; // T for trees | L for lake | P for potion
	public:
		void SetType(char Ty) {
			Type = Ty;
		}
		char GetType() {
			return Type;
		}
		//...........
		Obstacle(char Ty, char o) {
			Type = Ty;
			T = o;
		}
};
