function InitMap(environment)
	MapObject = ActorObjectInfo(1)
	MapObject:SetRenderType(1)
	MapObject.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/CELEBRA2.osgb"
	MapObject.DisplayDesc.UseLight = false
	MapObject:SetModelState(1)
	MapObject.PhysicDesc.Pos.X = 0
	MapObject.PhysicDesc.Pos.Y = 0
	MapObject.PhysicDesc.Pos.Z = 0
	MapObject.PhysicDesc.Pos.Rotation = 0
	MapObject:SetPhysicalActorType(1)
	MapObject:SetPhysicalShape(5)
	MapObject.PhysicDesc.Collidable = true
	MapObject.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/CELEBRA2.phy"
	MapObjectH = ActorHandler(MapObject)
	environment:AddActorObject(MapObjectH)


	OBJ_2 = ActorObjectInfo(2)
	OBJ_2:SetRenderType(1)
	OBJ_2.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_1.osgb"
	OBJ_2.DisplayDesc.UseLight = false
	OBJ_2:SetModelState(1)
	OBJ_2.PhysicDesc.Pos.X = 452.75
	OBJ_2.PhysicDesc.Pos.Y = 23.6719
	OBJ_2.PhysicDesc.Pos.Z = 585.953
	OBJ_2.PhysicDesc.Pos.Rotation = 90
	OBJ_2:SetPhysicalActorType(1)
	OBJ_2:SetPhysicalShape(5)
	OBJ_2.PhysicDesc.Collidable = true
	OBJ_2.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_1.phy"
	OBJ_2H = ActorHandler(OBJ_2)
	environment:AddActorObject(OBJ_2H)


	OBJ_3 = ActorObjectInfo(3)
	OBJ_3:SetRenderType(1)
	OBJ_3.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_2.osgb"
	OBJ_3.DisplayDesc.UseLight = false
	OBJ_3:SetModelState(1)
	OBJ_3.PhysicDesc.Pos.X = 452.75
	OBJ_3.PhysicDesc.Pos.Y = 23.6719
	OBJ_3.PhysicDesc.Pos.Z = 585.953
	OBJ_3.PhysicDesc.Pos.Rotation = 90
	OBJ_3:SetPhysicalActorType(1)
	OBJ_3:SetPhysicalShape(5)
	OBJ_3.PhysicDesc.Collidable = true
	OBJ_3.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_2.phy"
	OBJ_3H = ActorHandler(OBJ_3)
	environment:AddActorObject(OBJ_3H)


	OBJ_4 = ActorObjectInfo(4)
	OBJ_4:SetRenderType(1)
	OBJ_4.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_3.osgb"
	OBJ_4.DisplayDesc.UseLight = false
	OBJ_4:SetModelState(1)
	OBJ_4.PhysicDesc.Pos.X = 423.5
	OBJ_4.PhysicDesc.Pos.Y = 1.64063
	OBJ_4.PhysicDesc.Pos.Z = 598.5
	OBJ_4.PhysicDesc.Pos.Rotation = 270
	OBJ_4:SetPhysicalActorType(1)
	OBJ_4:SetPhysicalShape(5)
	OBJ_4.PhysicDesc.Collidable = true
	OBJ_4.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_3.phy"
	OBJ_4H = ActorHandler(OBJ_4)
	environment:AddActorObject(OBJ_4H)


	OBJ_5 = ActorObjectInfo(5)
	OBJ_5:SetRenderType(1)
	OBJ_5.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_3.osgb"
	OBJ_5.DisplayDesc.UseLight = false
	OBJ_5:SetModelState(1)
	OBJ_5.PhysicDesc.Pos.X = 423.5
	OBJ_5.PhysicDesc.Pos.Y = 1.64063
	OBJ_5.PhysicDesc.Pos.Z = 602.5
	OBJ_5.PhysicDesc.Pos.Rotation = 270
	OBJ_5:SetPhysicalActorType(1)
	OBJ_5:SetPhysicalShape(5)
	OBJ_5.PhysicDesc.Collidable = true
	OBJ_5.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_3.phy"
	OBJ_5H = ActorHandler(OBJ_5)
	environment:AddActorObject(OBJ_5H)


	OBJ_6 = ActorObjectInfo(6)
	OBJ_6:SetRenderType(1)
	OBJ_6.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_4.osgb"
	OBJ_6.DisplayDesc.UseLight = false
	OBJ_6:SetModelState(1)
	OBJ_6.PhysicDesc.Pos.X = 432.5
	OBJ_6.PhysicDesc.Pos.Y = 1.64063
	OBJ_6.PhysicDesc.Pos.Z = 597.625
	OBJ_6.PhysicDesc.Pos.Rotation = 270
	OBJ_6:SetPhysicalActorType(1)
	OBJ_6:SetPhysicalShape(5)
	OBJ_6.PhysicDesc.Collidable = true
	OBJ_6.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_4.phy"
	OBJ_6H = ActorHandler(OBJ_6)
	environment:AddActorObject(OBJ_6H)


	OBJ_7 = ActorObjectInfo(7)
	OBJ_7:SetRenderType(1)
	OBJ_7.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_4.osgb"
	OBJ_7.DisplayDesc.UseLight = false
	OBJ_7:SetModelState(1)
	OBJ_7.PhysicDesc.Pos.X = 441.5
	OBJ_7.PhysicDesc.Pos.Y = 1.64063
	OBJ_7.PhysicDesc.Pos.Z = 597.625
	OBJ_7.PhysicDesc.Pos.Rotation = 270
	OBJ_7:SetPhysicalActorType(1)
	OBJ_7:SetPhysicalShape(5)
	OBJ_7.PhysicDesc.Collidable = true
	OBJ_7.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_4.phy"
	OBJ_7H = ActorHandler(OBJ_7)
	environment:AddActorObject(OBJ_7H)


	OBJ_8 = ActorObjectInfo(8)
	OBJ_8:SetRenderType(1)
	OBJ_8.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_4.osgb"
	OBJ_8.DisplayDesc.UseLight = false
	OBJ_8:SetModelState(1)
	OBJ_8.PhysicDesc.Pos.X = 437.5
	OBJ_8.PhysicDesc.Pos.Y = 1.64063
	OBJ_8.PhysicDesc.Pos.Z = 603
	OBJ_8.PhysicDesc.Pos.Rotation = 270
	OBJ_8:SetPhysicalActorType(1)
	OBJ_8:SetPhysicalShape(5)
	OBJ_8.PhysicDesc.Collidable = true
	OBJ_8.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_4.phy"
	OBJ_8H = ActorHandler(OBJ_8)
	environment:AddActorObject(OBJ_8H)


	OBJ_9 = ActorObjectInfo(9)
	OBJ_9:SetRenderType(1)
	OBJ_9.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_5.osgb"
	OBJ_9.DisplayDesc.UseLight = false
	OBJ_9:SetModelState(1)
	OBJ_9.PhysicDesc.Pos.X = 444.5
	OBJ_9.PhysicDesc.Pos.Y = 1.64063
	OBJ_9.PhysicDesc.Pos.Z = 603
	OBJ_9.PhysicDesc.Pos.Rotation = 270
	OBJ_9:SetPhysicalActorType(1)
	OBJ_9:SetPhysicalShape(5)
	OBJ_9.PhysicDesc.Collidable = true
	OBJ_9.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_5.phy"
	OBJ_9H = ActorHandler(OBJ_9)
	environment:AddActorObject(OBJ_9H)


	OBJ_10 = ActorObjectInfo(10)
	OBJ_10:SetRenderType(1)
	OBJ_10.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_5.osgb"
	OBJ_10.DisplayDesc.UseLight = false
	OBJ_10:SetModelState(1)
	OBJ_10.PhysicDesc.Pos.X = 430.75
	OBJ_10.PhysicDesc.Pos.Y = 1.64063
	OBJ_10.PhysicDesc.Pos.Z = 603
	OBJ_10.PhysicDesc.Pos.Rotation = 270
	OBJ_10:SetPhysicalActorType(1)
	OBJ_10:SetPhysicalShape(5)
	OBJ_10.PhysicDesc.Collidable = true
	OBJ_10.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_5.phy"
	OBJ_10H = ActorHandler(OBJ_10)
	environment:AddActorObject(OBJ_10H)


	OBJ_11 = ActorObjectInfo(11)
	OBJ_11:SetRenderType(1)
	OBJ_11.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_6.osgb"
	OBJ_11.DisplayDesc.UseLight = false
	OBJ_11:SetModelState(1)
	OBJ_11.PhysicDesc.Pos.X = 430.75
	OBJ_11.PhysicDesc.Pos.Y = 1.64063
	OBJ_11.PhysicDesc.Pos.Z = 603
	OBJ_11.PhysicDesc.Pos.Rotation = 270
	OBJ_11:SetPhysicalActorType(1)
	OBJ_11:SetPhysicalShape(5)
	OBJ_11.PhysicDesc.Collidable = true
	OBJ_11.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_6.phy"
	OBJ_11H = ActorHandler(OBJ_11)
	environment:AddActorObject(OBJ_11H)


	OBJ_12 = ActorObjectInfo(12)
	OBJ_12:SetRenderType(1)
	OBJ_12.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_6.osgb"
	OBJ_12.DisplayDesc.UseLight = false
	OBJ_12:SetModelState(1)
	OBJ_12.PhysicDesc.Pos.X = 444.5
	OBJ_12.PhysicDesc.Pos.Y = 1.64063
	OBJ_12.PhysicDesc.Pos.Z = 603
	OBJ_12.PhysicDesc.Pos.Rotation = 270
	OBJ_12:SetPhysicalActorType(1)
	OBJ_12:SetPhysicalShape(5)
	OBJ_12.PhysicDesc.Collidable = true
	OBJ_12.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_6.phy"
	OBJ_12H = ActorHandler(OBJ_12)
	environment:AddActorObject(OBJ_12H)


	OBJ_13 = ActorObjectInfo(13)
	OBJ_13:SetRenderType(1)
	OBJ_13.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_4.osgb"
	OBJ_13.DisplayDesc.UseLight = false
	OBJ_13:SetModelState(1)
	OBJ_13.PhysicDesc.Pos.X = 433.5
	OBJ_13.PhysicDesc.Pos.Y = 22.5
	OBJ_13.PhysicDesc.Pos.Z = 583.5
	OBJ_13.PhysicDesc.Pos.Rotation = 270
	OBJ_13:SetPhysicalActorType(1)
	OBJ_13:SetPhysicalShape(5)
	OBJ_13.PhysicDesc.Collidable = true
	OBJ_13.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_4.phy"
	OBJ_13H = ActorHandler(OBJ_13)
	environment:AddActorObject(OBJ_13H)


	OBJ_14 = ActorObjectInfo(14)
	OBJ_14:SetRenderType(1)
	OBJ_14.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_4.osgb"
	OBJ_14.DisplayDesc.UseLight = false
	OBJ_14:SetModelState(1)
	OBJ_14.PhysicDesc.Pos.X = 465
	OBJ_14.PhysicDesc.Pos.Y = 22.5
	OBJ_14.PhysicDesc.Pos.Z = 588.25
	OBJ_14.PhysicDesc.Pos.Rotation = 270
	OBJ_14:SetPhysicalActorType(1)
	OBJ_14:SetPhysicalShape(5)
	OBJ_14.PhysicDesc.Collidable = true
	OBJ_14.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_4.phy"
	OBJ_14H = ActorHandler(OBJ_14)
	environment:AddActorObject(OBJ_14H)


	OBJ_15 = ActorObjectInfo(15)
	OBJ_15:SetRenderType(1)
	OBJ_15.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_7.osgb"
	OBJ_15.DisplayDesc.UseLight = false
	OBJ_15:SetModelState(1)
	OBJ_15.PhysicDesc.Pos.X = 463
	OBJ_15.PhysicDesc.Pos.Y = 24.8438
	OBJ_15.PhysicDesc.Pos.Z = 584
	OBJ_15.PhysicDesc.Pos.Rotation = 270
	OBJ_15:SetPhysicalActorType(1)
	OBJ_15:SetPhysicalShape(5)
	OBJ_15.PhysicDesc.Collidable = true
	OBJ_15.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_7.phy"
	OBJ_15H = ActorHandler(OBJ_15)
	environment:AddActorObject(OBJ_15H)


	OBJ_16 = ActorObjectInfo(16)
	OBJ_16:SetRenderType(1)
	OBJ_16.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_8.osgb"
	OBJ_16.DisplayDesc.UseLight = false
	OBJ_16:SetModelState(1)
	OBJ_16.PhysicDesc.Pos.X = 460.25
	OBJ_16.PhysicDesc.Pos.Y = 24.8438
	OBJ_16.PhysicDesc.Pos.Z = 580.375
	OBJ_16.PhysicDesc.Pos.Rotation = 270
	OBJ_16:SetPhysicalActorType(1)
	OBJ_16:SetPhysicalShape(5)
	OBJ_16.PhysicDesc.Collidable = true
	OBJ_16.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_8.phy"
	OBJ_16H = ActorHandler(OBJ_16)
	environment:AddActorObject(OBJ_16H)


	OBJ_17 = ActorObjectInfo(17)
	OBJ_17:SetRenderType(1)
	OBJ_17.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_9.osgb"
	OBJ_17.DisplayDesc.UseLight = false
	OBJ_17:SetModelState(1)
	OBJ_17.PhysicDesc.Pos.X = 459.25
	OBJ_17.PhysicDesc.Pos.Y = 23.6719
	OBJ_17.PhysicDesc.Pos.Z = 587
	OBJ_17.PhysicDesc.Pos.Rotation = 270
	OBJ_17:SetPhysicalActorType(1)
	OBJ_17:SetPhysicalShape(5)
	OBJ_17.PhysicDesc.Collidable = true
	OBJ_17.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_9.phy"
	OBJ_17H = ActorHandler(OBJ_17)
	environment:AddActorObject(OBJ_17H)


	OBJ_18 = ActorObjectInfo(18)
	OBJ_18:SetRenderType(1)
	OBJ_18.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_10.osgb"
	OBJ_18.DisplayDesc.UseLight = false
	OBJ_18:SetModelState(1)
	OBJ_18.PhysicDesc.Pos.X = 463.625
	OBJ_18.PhysicDesc.Pos.Y = 24.8438
	OBJ_18.PhysicDesc.Pos.Z = 577.125
	OBJ_18.PhysicDesc.Pos.Rotation = 270
	OBJ_18:SetPhysicalActorType(1)
	OBJ_18:SetPhysicalShape(5)
	OBJ_18.PhysicDesc.Collidable = true
	OBJ_18.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_10.phy"
	OBJ_18H = ActorHandler(OBJ_18)
	environment:AddActorObject(OBJ_18H)


	OBJ_19 = ActorObjectInfo(19)
	OBJ_19:SetRenderType(1)
	OBJ_19.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_11.osgb"
	OBJ_19.DisplayDesc.UseLight = false
	OBJ_19:SetModelState(1)
	OBJ_19.PhysicDesc.Pos.X = 452.75
	OBJ_19.PhysicDesc.Pos.Y = 23.6719
	OBJ_19.PhysicDesc.Pos.Z = 585.953
	OBJ_19.PhysicDesc.Pos.Rotation = 90
	OBJ_19:SetPhysicalActorType(1)
	OBJ_19:SetPhysicalShape(5)
	OBJ_19.PhysicDesc.Collidable = true
	OBJ_19.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_11.phy"
	OBJ_19H = ActorHandler(OBJ_19)
	environment:AddActorObject(OBJ_19H)


	OBJ_20 = ActorObjectInfo(20)
	OBJ_20:SetRenderType(1)
	OBJ_20.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_12.osgb"
	OBJ_20.DisplayDesc.UseLight = false
	OBJ_20:SetModelState(1)
	OBJ_20.PhysicDesc.Pos.X = 452.75
	OBJ_20.PhysicDesc.Pos.Y = 23.6719
	OBJ_20.PhysicDesc.Pos.Z = 585.953
	OBJ_20.PhysicDesc.Pos.Rotation = 90
	OBJ_20:SetPhysicalActorType(1)
	OBJ_20:SetPhysicalShape(5)
	OBJ_20.PhysicDesc.Collidable = true
	OBJ_20.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_12.phy"
	OBJ_20H = ActorHandler(OBJ_20)
	environment:AddActorObject(OBJ_20H)


	OBJ_21 = ActorObjectInfo(21)
	OBJ_21:SetRenderType(1)
	OBJ_21.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_13.osgb"
	OBJ_21.DisplayDesc.UseLight = false
	OBJ_21:SetModelState(1)
	OBJ_21.PhysicDesc.Pos.X = 449.25
	OBJ_21.PhysicDesc.Pos.Y = 15.1992
	OBJ_21.PhysicDesc.Pos.Z = 573.5
	OBJ_21.PhysicDesc.Pos.Rotation = 360
	OBJ_21:SetPhysicalActorType(1)
	OBJ_21:SetPhysicalShape(5)
	OBJ_21.PhysicDesc.Collidable = true
	OBJ_21.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_13.phy"
	OBJ_21H = ActorHandler(OBJ_21)
	environment:AddActorObject(OBJ_21H)


	OBJ_22 = ActorObjectInfo(22)
	OBJ_22:SetRenderType(1)
	OBJ_22.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_14.osgb"
	OBJ_22.DisplayDesc.UseLight = false
	OBJ_22:SetModelState(1)
	OBJ_22.PhysicDesc.Pos.X = 452.75
	OBJ_22.PhysicDesc.Pos.Y = 23.6719
	OBJ_22.PhysicDesc.Pos.Z = 585.953
	OBJ_22.PhysicDesc.Pos.Rotation = 90
	OBJ_22:SetPhysicalActorType(1)
	OBJ_22:SetPhysicalShape(5)
	OBJ_22.PhysicDesc.Collidable = true
	OBJ_22.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_14.phy"
	OBJ_22H = ActorHandler(OBJ_22)
	environment:AddActorObject(OBJ_22H)


	OBJ_23 = ActorObjectInfo(23)
	OBJ_23:SetRenderType(1)
	OBJ_23.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_15.osgb"
	OBJ_23.DisplayDesc.UseLight = false
	OBJ_23:SetModelState(1)
	OBJ_23.PhysicDesc.Pos.X = 461.5
	OBJ_23.PhysicDesc.Pos.Y = 24.6094
	OBJ_23.PhysicDesc.Pos.Z = 573.75
	OBJ_23.PhysicDesc.Pos.Rotation = 270
	OBJ_23:SetPhysicalActorType(1)
	OBJ_23:SetPhysicalShape(5)
	OBJ_23.PhysicDesc.Collidable = true
	OBJ_23.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_15.phy"
	OBJ_23H = ActorHandler(OBJ_23)
	environment:AddActorObject(OBJ_23H)


	OBJ_24 = ActorObjectInfo(24)
	OBJ_24:SetRenderType(1)
	OBJ_24.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_16.osgb"
	OBJ_24.DisplayDesc.UseLight = false
	OBJ_24:SetModelState(1)
	OBJ_24.PhysicDesc.Pos.X = 461.125
	OBJ_24.PhysicDesc.Pos.Y = 17.5781
	OBJ_24.PhysicDesc.Pos.Z = 558.625
	OBJ_24.PhysicDesc.Pos.Rotation = 270
	OBJ_24:SetPhysicalActorType(1)
	OBJ_24:SetPhysicalShape(5)
	OBJ_24.PhysicDesc.Collidable = true
	OBJ_24.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_16.phy"
	OBJ_24H = ActorHandler(OBJ_24)
	environment:AddActorObject(OBJ_24H)


	OBJ_25 = ActorObjectInfo(25)
	OBJ_25:SetRenderType(1)
	OBJ_25.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_17.osgb"
	OBJ_25.DisplayDesc.UseLight = false
	OBJ_25:SetModelState(1)
	OBJ_25.PhysicDesc.Pos.X = 449.25
	OBJ_25.PhysicDesc.Pos.Y = 15.1992
	OBJ_25.PhysicDesc.Pos.Z = 573.5
	OBJ_25.PhysicDesc.Pos.Rotation = 360
	OBJ_25:SetPhysicalActorType(1)
	OBJ_25:SetPhysicalShape(5)
	OBJ_25.PhysicDesc.Collidable = true
	OBJ_25.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_17.phy"
	OBJ_25H = ActorHandler(OBJ_25)
	environment:AddActorObject(OBJ_25H)


	OBJ_26 = ActorObjectInfo(26)
	OBJ_26:SetRenderType(1)
	OBJ_26.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_18.osgb"
	OBJ_26.DisplayDesc.UseLight = false
	OBJ_26:SetModelState(1)
	OBJ_26.PhysicDesc.Pos.X = 449.25
	OBJ_26.PhysicDesc.Pos.Y = 15.1992
	OBJ_26.PhysicDesc.Pos.Z = 573.5
	OBJ_26.PhysicDesc.Pos.Rotation = 360
	OBJ_26:SetPhysicalActorType(1)
	OBJ_26:SetPhysicalShape(5)
	OBJ_26.PhysicDesc.Collidable = true
	OBJ_26.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_18.phy"
	OBJ_26H = ActorHandler(OBJ_26)
	environment:AddActorObject(OBJ_26H)


	OBJ_27 = ActorObjectInfo(27)
	OBJ_27:SetRenderType(1)
	OBJ_27.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_19.osgb"
	OBJ_27.DisplayDesc.UseLight = false
	OBJ_27:SetModelState(1)
	OBJ_27.PhysicDesc.Pos.X = 449.25
	OBJ_27.PhysicDesc.Pos.Y = 15.1992
	OBJ_27.PhysicDesc.Pos.Z = 573.5
	OBJ_27.PhysicDesc.Pos.Rotation = 360
	OBJ_27:SetPhysicalActorType(1)
	OBJ_27:SetPhysicalShape(5)
	OBJ_27.PhysicDesc.Collidable = true
	OBJ_27.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_19.phy"
	OBJ_27H = ActorHandler(OBJ_27)
	environment:AddActorObject(OBJ_27H)


	OBJ_28 = ActorObjectInfo(28)
	OBJ_28:SetRenderType(1)
	OBJ_28.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_20.osgb"
	OBJ_28.DisplayDesc.UseLight = false
	OBJ_28:SetModelState(1)
	OBJ_28.PhysicDesc.Pos.X = 449.25
	OBJ_28.PhysicDesc.Pos.Y = 15.1992
	OBJ_28.PhysicDesc.Pos.Z = 573.5
	OBJ_28.PhysicDesc.Pos.Rotation = 360
	OBJ_28:SetPhysicalActorType(1)
	OBJ_28:SetPhysicalShape(5)
	OBJ_28.PhysicDesc.Collidable = true
	OBJ_28.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_20.phy"
	OBJ_28H = ActorHandler(OBJ_28)
	environment:AddActorObject(OBJ_28H)


	OBJ_29 = ActorObjectInfo(29)
	OBJ_29:SetRenderType(1)
	OBJ_29.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_21.osgb"
	OBJ_29.DisplayDesc.UseLight = false
	OBJ_29:SetModelState(1)
	OBJ_29.PhysicDesc.Pos.X = 449.5
	OBJ_29.PhysicDesc.Pos.Y = 15
	OBJ_29.PhysicDesc.Pos.Z = 568.104
	OBJ_29.PhysicDesc.Pos.Rotation = 180
	OBJ_29:SetPhysicalActorType(1)
	OBJ_29:SetPhysicalShape(5)
	OBJ_29.PhysicDesc.Collidable = true
	OBJ_29.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_21.phy"
	OBJ_29H = ActorHandler(OBJ_29)
	environment:AddActorObject(OBJ_29H)


	OBJ_30 = ActorObjectInfo(30)
	OBJ_30:SetRenderType(1)
	OBJ_30.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_22.osgb"
	OBJ_30.DisplayDesc.UseLight = false
	OBJ_30:SetModelState(1)
	OBJ_30.PhysicDesc.Pos.X = 449.25
	OBJ_30.PhysicDesc.Pos.Y = 15.1992
	OBJ_30.PhysicDesc.Pos.Z = 573.5
	OBJ_30.PhysicDesc.Pos.Rotation = 360
	OBJ_30:SetPhysicalActorType(1)
	OBJ_30:SetPhysicalShape(5)
	OBJ_30.PhysicDesc.Collidable = true
	OBJ_30.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_22.phy"
	OBJ_30H = ActorHandler(OBJ_30)
	environment:AddActorObject(OBJ_30H)


	OBJ_31 = ActorObjectInfo(31)
	OBJ_31:SetRenderType(1)
	OBJ_31.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_23.osgb"
	OBJ_31.DisplayDesc.UseLight = false
	OBJ_31:SetModelState(1)
	OBJ_31.PhysicDesc.Pos.X = 464
	OBJ_31.PhysicDesc.Pos.Y = 19.9219
	OBJ_31.PhysicDesc.Pos.Z = 565
	OBJ_31.PhysicDesc.Pos.Rotation = 90
	OBJ_31:SetPhysicalActorType(1)
	OBJ_31:SetPhysicalShape(5)
	OBJ_31.PhysicDesc.Collidable = true
	OBJ_31.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_23.phy"
	OBJ_31H = ActorHandler(OBJ_31)
	environment:AddActorObject(OBJ_31H)


	OBJ_32 = ActorObjectInfo(32)
	OBJ_32:SetRenderType(1)
	OBJ_32.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_24.osgb"
	OBJ_32.DisplayDesc.UseLight = false
	OBJ_32:SetModelState(1)
	OBJ_32.PhysicDesc.Pos.X = 437.75
	OBJ_32.PhysicDesc.Pos.Y = 1.64063
	OBJ_32.PhysicDesc.Pos.Z = 597
	OBJ_32.PhysicDesc.Pos.Rotation = 270
	OBJ_32:SetPhysicalActorType(1)
	OBJ_32:SetPhysicalShape(5)
	OBJ_32.PhysicDesc.Collidable = true
	OBJ_32.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/CELEBRA2_OBJ_24.phy"
	OBJ_32H = ActorHandler(OBJ_32)
	environment:AddActorObject(OBJ_32H)
end
