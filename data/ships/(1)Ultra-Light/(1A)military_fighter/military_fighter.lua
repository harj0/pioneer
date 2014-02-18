-- Copyright © 2008-2014 Pioneer Developers. See AUTHORS.txt for details
-- Licensed under the terms of CC-BY-SA 3.0. See licenses/CC-BY-SA-3.0.txt

define_ship {
	name='Falcon',
	ship_class = 'military_fighter',
	manufacturer = 'Arcor',
	model = 'military_fighter',
	forward_thrust = 16e5,
	reverse_thrust = 16e5,
	up_thrust = 160e5,
	down_thrust = 160e5,
	left_thrust = 160e5,
	right_thrust = 160e5,
	angular_thrust = 80e5,
	max_cargo = 9,
	max_missile = 8,
	max_fuelscoop = 0,
	max_cargoscoop = 0,
	min_crew = 1,
	max_crew = 1,
	capacity = 9,
	hull_mass = 5,
	fuel_tank_mass = 2,
	-- Exhaust velocity Vc [m/s] is equivalent of engine efficiency and depend on used technology. Higher Vc means lower fuel consumption.
	-- Smaller ships built for speed often mount engines with higher Vc. Another way to make faster ship is to increase fuel_tank_mass.
	effective_exhaust_velocity = 120000e3,
	price = 0,
	hyperdrive_class = 0,
	-- Paragon Flight System
    max_maneuver_speed = 1000,
}
