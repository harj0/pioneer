-- Copyright © 2013-2014 Meteoric Games Ltd

--Ships not available for purchase (ambient ships)
define_ship {
	name='leviathan supertanker',
	ship_class = 'tanker',
	manufacturer = 'alders_vectrum',
	model='tanker_ultraheavy',
	forward_thrust = 39000e5,
	reverse_thrust = 39000e5,
	up_thrust = 13000e5,
	down_thrust = 13000e5,
	left_thrust = 13000e5,
	right_thrust = 13000e5,
	angular_thrust = 130000e5,
	max_cargo = 23868,
	max_laser = 0,
	max_missile = 0,
	max_cargoscoop = 0,
	min_crew = 6,
	max_crew = 20,
	capacity = 23868,
	hull_mass = 11934,
	fuel_tank_mass = 3978,
	-- Exhaust velocity Vc [m/s] is equivalent of engine efficiency and depend on used technology. Higher Vc means lower fuel consumption.
	-- Smaller ships built for speed often mount engines with higher Vc. Another way to make faster ship is to increase fuel_tank_mass.
	effective_exhaust_velocity = 55123e3,
	price = 0,
	hyperdrive_class = 12,
	-- Paragon Flight System
    max_maneuver_speed = 100,
}
