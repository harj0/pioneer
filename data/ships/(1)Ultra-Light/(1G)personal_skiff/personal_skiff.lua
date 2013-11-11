-- Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
-- Licensed under the terms of CC-BY-SA 3.0. See licenses/CC-BY-SA-3.0.txt

define_ship {
	name='Personal Skiff',
	model='personal_skiff',
	forward_thrust = 51e5,
	reverse_thrust = 51e5,
	up_thrust = 27e5,
	down_thrust = 14e5,
	left_thrust = 14e5,
	right_thrust = 14e5,
	angular_thrust = 36e5,
	max_cargo = 12,
	max_laser = 0,
	max_missile = 0,
	max_fuelscoop = 0,
	max_cargoscoop = 0,
	max_ecm = 0,
	max_engine = 0,
	min_crew = 1,
	max_crew = 1,
	hyperdrive_class = 0,
	capacity = 12,
	hull_mass = 8,
	fuel_tank_mass = 15,
	-- Exhaust velocity Vc [m/s] is equivalent of engine efficiency and depend on used technology. Higher Vc means lower fuel consumption.
	-- Smaller ships built for speed often mount engines with higher Vc. Another way to make faster ship is to increase fuel_tank_mass.
	effective_exhaust_velocity = 81103e3,
	price = 40000,
        -- Paragon Flight System
        max_maneuver_speed = 10e3,
}
