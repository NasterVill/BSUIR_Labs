#pragma once

#include "Vehicle.h"

class SpaceVehicle : virtual public Vehicle
{
	public:
		SpaceVehicle();
		SpaceVehicle(int, int);
		~SpaceVehicle();

		int get_sv_mass();
		int get_engine_power();

		int computeCost();
		void show();
	private:
		int _sv_mass;
		int _engine_power;								// ��� �������� ��������� ��� ����������� �������� ���������
		static const int _engine_coefficient;			// � ����������� ����������� ��������\���������
		static const int _mass_coefficient;
};