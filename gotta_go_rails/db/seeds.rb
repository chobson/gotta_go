# This file should contain all the record creation needed to seed the database with its default values.
# The data can then be loaded with the rake db:seed (or created alongside the db with db:setup).
#
# Examples:
#
#   cities = City.create([{ name: 'Chicago' }, { name: 'Copenhagen' }])
#   Mayor.create(name: 'Emanuel', city: cities.first)

[Restroom, Sensor, Activity].each(&:delete_all)

photocell_restroom = Restroom.create!({name: "Photocell Restroom"})
photocell_restroom.sensors.create!({name: "Photocell Sensor", token: "photocell"})

pir_restroom = Restroom.create!({name: "PIR Restroom"})
pir_restroom.sensors.create!({name: "PIR Sensor", token: "pir"})

ir_restroom = Restroom.create!({name: "IR Restroom"})
ir_restroom.sensors.create!({name: "IR Sensor", token: "ir"})

sonar_restroom = Restroom.create!({name: "Sonar Restroom"})
sonar_restroom.sensors.create!({name: "Sonar Sensor", token: "sonar"})
