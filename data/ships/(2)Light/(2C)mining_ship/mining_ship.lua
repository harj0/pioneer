-- Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
-- Licensed under the terms of CC-BY-SA 3.0. See licenses/CC-BY-SA-3.0.txt

define_ship {
	name='Mining Ship',
	ship_class = 'mining_ship',
	manufacturer = 'alders_vectrum',
	model='mining_ship',
	forward_thrust = 1323e5,
	reverse_thrust = 1323e5,
	up_thrust = 13230e5,
	down_thrust = 13230e5,
	left_thrust = 13230e5,
	right_thrust = 13230e5,
	angular_thrust = 500e5,
	max_cargo = 792,
	max_missile = 1,
	max_laser = 0,
	max_cargoscoop = 0,
	max_fuelscoop = 1,
	min_crew = 1,
	max_crew = 2,
	capacity = 792,
	hull_mass = 396,
	fuel_tank_mass = 132,
	-- Exhaust velocity Vc [m/s] is equivalent of engine efficiency and depend on used technology. Higher Vc means lower fuel consumption.
	-- Smaller ships built for speed often mount engines with higher Vc. Another way to make faster ship is to increase fuel_tank_mass.
	effective_exhaust_velocity = 57273e3,
	price = 450000,
	hyperdrive_class = 9,
	-- Paragon Flight System
    max_maneuver_speed = 400,
}
