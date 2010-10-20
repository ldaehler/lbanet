function InitGlobal(environment)
	Action_1 = TeleportAction(1, "tp_from_main_to_secondary", "Map002_2", 0)
	environment:AddAction(Action_1)

	Action_2 = TeleportAction(2, "tp_from_main_to_citadel", "CITABAU", 2)
	environment:AddAction(Action_2)

	Action_3 = TeleportAction(3, "tp_from_citadel_to_main", "Map001_2", 2)
	environment:AddAction(Action_3)

	Action_4 = TeleportAction(4, "tp_from_secondary_to_main", "Map001_2", 3)
	environment:AddAction(Action_4)

	Action_5 = TeleportAction(5, "tp_from_architect_to_citadel", "CITABAU", 3)
	environment:AddAction(Action_5)

	Action_6 = TeleportAction(6, "tp_from_citadel_to_architect", "Map010_2", 0)
	environment:AddAction(Action_6)

	Action_7 = TeleportAction(7, "tp_from_ticket_to_citadel", "CITABAU", 4)
	environment:AddAction(Action_7)

	Action_8 = TeleportAction(8, "tp_from_citadel_to_ticket", "Map009_2", 0)
	environment:AddAction(Action_8)

	Action_9 = TeleportAction(9, "tp_from_mrpaul_to_citadel", "CITABAU", 5)
	environment:AddAction(Action_9)

	Action_10 = TeleportAction(10, "tp_from_citadel_to_mrpaul", "Map008_2", 0)
	environment:AddAction(Action_10)

	Action_11 = TeleportAction(11, "tp_from_citadel_to_shop", "Map015_2", 0)
	environment:AddAction(Action_11)

	Action_12 = TeleportAction(12, "tp_from_shop_to_citadel", "CITABAU", 6)
	environment:AddAction(Action_12)

	Action_13 = TeleportAction(13, "tp_from_citadel_to_tavern", "Map004_2", 0)
	environment:AddAction(Action_13)

	Action_14 = TeleportAction(14, "from_tavern_to_citadel", "CITABAU", 7)
	environment:AddAction(Action_14)

	Action_15 = TeleportAction(15, "tp_from_citadel_to_museum", "Map016_2", 0)
	environment:AddAction(Action_15)

	Action_16 = TeleportAction(16, "tp_from_museum_to_citadel", "CITABAU", 8)
	environment:AddAction(Action_16)

	Action_17 = TeleportAction(17, "tp_from_tavern_to_taverncellar", "Map005_2", 0)
	environment:AddAction(Action_17)

	Action_18 = TeleportAction(18, "tp_from_taverncellar_to_tavern", "Map004_2", 1)
	environment:AddAction(Action_18)

	Action_19 = TeleportAction(19, "tp_sendell", "Map035_2", 1)
	environment:AddAction(Action_19)

	Action_20 = TeleportAction(20, "tp_from_taverncellar_to_sewer", "Map018_2", 0)
	environment:AddAction(Action_20)

	Action_21 = TeleportAction(21, "tp_from_sewer_to_sewersecret", "Map019_2", 1)
	environment:AddAction(Action_21)

	Action_22 = TeleportAction(22, "tp_from_sewersecret_to_sewer", "Map018_2", 3)
	environment:AddAction(Action_22)

	Action_23 = TeleportAction(23, "tp_from_citadel_to_pharmacy", "Map023_2", 1)
	environment:AddAction(Action_23)

	Action_24 = TeleportAction(24, "tp_from_pharmacy_to_citadel", "CITABAU", 9)
	environment:AddAction(Action_24)

	Action_25 = TeleportAction(25, "tp_from_citadel_to_baggage", "Map006_2", 1)
	environment:AddAction(Action_25)

	Action_26 = TeleportAction(26, "tp_from_baggage_to_citadel", "CITABAU", 10)
	environment:AddAction(Action_26)

	Action_27 = TeleportAction(27, "tp_from_baggagebag_to_baggagesecondbag", "Map007_2", 1)
	environment:AddAction(Action_27)

	Action_28 = TeleportAction(28, "tp_from_baggagesecondbag_to_baggagebag", "Map006_2", 2)
	environment:AddAction(Action_28)

	Action_29 = TeleportAction(29, "tp_from_baggagesecond_to_baggage", "Map006_2", 3)
	environment:AddAction(Action_29)

	Action_30 = TeleportAction(30, "tp_from_baggage_to_baggagesecond", "Map007_2", 2)
	environment:AddAction(Action_30)

	Action_31 = TeleportAction(31, "tp_from_baggagesecond_to_baggagesecret", "Map017_2", 1)
	environment:AddAction(Action_31)

	Action_32 = TeleportAction(32, "tp_from_baggagesecret_to_baggagesecond", "Map007_2", 3)
	environment:AddAction(Action_32)

	Action_33 = TeleportAction(33, "tp_from_citadel_to_school", "Map038_2", 1)
	environment:AddAction(Action_33)

	Action_34 = TeleportAction(34, "tp_from_school_to_citadel", "CITABAU", 11)
	environment:AddAction(Action_34)

	Action_35 = TeleportAction(35, "tp_from_citadel_to_magician", "Map022_2", 1)
	environment:AddAction(Action_35)

	Action_36 = TeleportAction(36, "tp_from_magician_to_citadel", "CITABAU", 12)
	environment:AddAction(Action_36)

	Action_37 = TeleportAction(37, "tp_from_spidercavefirst_to_citadelfirst", "CITABAU", 13)
	environment:AddAction(Action_37)

	Action_38 = TeleportAction(38, "tp_from_citadel_to_spidercavefirst", "Map043_2", 1)
	environment:AddAction(Action_38)

	Action_39 = TeleportAction(39, "tp_from_spidersecond_to_citadelsecond", "CITABAU", 14)
	environment:AddAction(Action_39)

	Action_40 = TeleportAction(40, "tp_from_citadelsecond_to_spidersecond", "Map043_2", 2)
	environment:AddAction(Action_40)

	Action_41 = TeleportAction(41, "tp_from_citadel_to_tralumain", "Map003_2", 1)
	environment:AddAction(Action_41)

	Action_42 = TeleportAction(42, "tp_from_tralumain_to_citadel", "CITABAU", 15)
	environment:AddAction(Action_42)

	Action_43 = TeleportAction(43, "tp_from_tralumain_to_tralusecond", "Map021_2", 1)
	environment:AddAction(Action_43)

	Action_44 = TeleportAction(44, "tp_from_tralusecond_to_tralumain", "Map003_2", 2)
	environment:AddAction(Action_44)

	Action_45 = TeleportAction(45, "tp_from_tralusecond_to_traluthird", "Map020_2", 1)
	environment:AddAction(Action_45)

	Action_46 = TeleportAction(46, "tp_from_traluthird_to_tralusecond", "Map021_2", 2)
	environment:AddAction(Action_46)

	Action_47 = TeleportAction(47, "tp_from_traluthird_to_tralumain", "Map003_2", 3)
	environment:AddAction(Action_47)

	Action_48 = TeleportAction(48, "tp_from_tralumain_to_traluthird", "Map020_2", 2)
	environment:AddAction(Action_48)

end
