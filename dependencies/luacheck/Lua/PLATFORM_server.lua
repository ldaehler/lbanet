function InitMap(environment)
	MapObject = ActorObjectInfo(1)
	MapObject:SetRenderType(1)
	MapObject.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM.osgb"
	MapObject.DisplayDesc.UseLight = false
	MapObject:SetModelState(1)
	MapObject.PhysicDesc.Pos.X = 0
	MapObject.PhysicDesc.Pos.Y = 0
	MapObject.PhysicDesc.Pos.Z = 0
	MapObject.PhysicDesc.Pos.Rotation = 0
	MapObject:SetPhysicalActorType(1)
	MapObject:SetPhysicalShape(5)
	MapObject.PhysicDesc.Collidable = true
	MapObject.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM.phy"
	MapObjectH = ActorHandler(MapObject)
	environment:AddActorObject(MapObjectH)


	OBJ_2 = ActorObjectInfo(2)
	OBJ_2:SetRenderType(1)
	OBJ_2.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_1.osgb"
	OBJ_2.DisplayDesc.UseLight = false
	OBJ_2:SetModelState(1)
	OBJ_2.PhysicDesc.Pos.X = 453
	OBJ_2.PhysicDesc.Pos.Y = 15.7422
	OBJ_2.PhysicDesc.Pos.Z = 584.979
	OBJ_2.PhysicDesc.Pos.Rotation = 90
	OBJ_2:SetPhysicalActorType(1)
	OBJ_2:SetPhysicalShape(5)
	OBJ_2.PhysicDesc.Collidable = true
	OBJ_2.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_1.phy"
	OBJ_2H = ActorHandler(OBJ_2)
	environment:AddActorObject(OBJ_2H)


	OBJ_3 = ActorObjectInfo(3)
	OBJ_3:SetRenderType(1)
	OBJ_3.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_2.osgb"
	OBJ_3.DisplayDesc.UseLight = false
	OBJ_3:SetModelState(1)
	OBJ_3.PhysicDesc.Pos.X = 453
	OBJ_3.PhysicDesc.Pos.Y = 15.7422
	OBJ_3.PhysicDesc.Pos.Z = 584.979
	OBJ_3.PhysicDesc.Pos.Rotation = 90
	OBJ_3:SetPhysicalActorType(1)
	OBJ_3:SetPhysicalShape(5)
	OBJ_3.PhysicDesc.Collidable = true
	OBJ_3.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_2.phy"
	OBJ_3H = ActorHandler(OBJ_3)
	environment:AddActorObject(OBJ_3H)


	OBJ_4 = ActorObjectInfo(4)
	OBJ_4:SetRenderType(1)
	OBJ_4.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_3.osgb"
	OBJ_4.DisplayDesc.UseLight = false
	OBJ_4:SetModelState(1)
	OBJ_4.PhysicDesc.Pos.X = 453
	OBJ_4.PhysicDesc.Pos.Y = 15.7422
	OBJ_4.PhysicDesc.Pos.Z = 584.979
	OBJ_4.PhysicDesc.Pos.Rotation = 90
	OBJ_4:SetPhysicalActorType(1)
	OBJ_4:SetPhysicalShape(5)
	OBJ_4.PhysicDesc.Collidable = true
	OBJ_4.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_3.phy"
	OBJ_4H = ActorHandler(OBJ_4)
	environment:AddActorObject(OBJ_4H)


	OBJ_5 = ActorObjectInfo(5)
	OBJ_5:SetRenderType(1)
	OBJ_5.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_4.osgb"
	OBJ_5.DisplayDesc.UseLight = false
	OBJ_5:SetModelState(1)
	OBJ_5.PhysicDesc.Pos.X = 432
	OBJ_5.PhysicDesc.Pos.Y = 9.375
	OBJ_5.PhysicDesc.Pos.Z = 573
	OBJ_5.PhysicDesc.Pos.Rotation = 90
	OBJ_5:SetPhysicalActorType(1)
	OBJ_5:SetPhysicalShape(5)
	OBJ_5.PhysicDesc.Collidable = true
	OBJ_5.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_4.phy"
	OBJ_5H = ActorHandler(OBJ_5)
	environment:AddActorObject(OBJ_5H)


	OBJ_6 = ActorObjectInfo(6)
	OBJ_6:SetRenderType(1)
	OBJ_6.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_5.osgb"
	OBJ_6.DisplayDesc.UseLight = false
	OBJ_6:SetModelState(1)
	OBJ_6.PhysicDesc.Pos.X = 432
	OBJ_6.PhysicDesc.Pos.Y = 9.375
	OBJ_6.PhysicDesc.Pos.Z = 573
	OBJ_6.PhysicDesc.Pos.Rotation = 90
	OBJ_6:SetPhysicalActorType(1)
	OBJ_6:SetPhysicalShape(5)
	OBJ_6.PhysicDesc.Collidable = true
	OBJ_6.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_5.phy"
	OBJ_6H = ActorHandler(OBJ_6)
	environment:AddActorObject(OBJ_6H)


	OBJ_7 = ActorObjectInfo(7)
	OBJ_7:SetRenderType(1)
	OBJ_7.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_6.osgb"
	OBJ_7.DisplayDesc.UseLight = false
	OBJ_7:SetModelState(1)
	OBJ_7.PhysicDesc.Pos.X = 432
	OBJ_7.PhysicDesc.Pos.Y = 9.375
	OBJ_7.PhysicDesc.Pos.Z = 573
	OBJ_7.PhysicDesc.Pos.Rotation = 90
	OBJ_7:SetPhysicalActorType(1)
	OBJ_7:SetPhysicalShape(5)
	OBJ_7.PhysicDesc.Collidable = true
	OBJ_7.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_6.phy"
	OBJ_7H = ActorHandler(OBJ_7)
	environment:AddActorObject(OBJ_7H)


	OBJ_8 = ActorObjectInfo(8)
	OBJ_8:SetRenderType(1)
	OBJ_8.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_7.osgb"
	OBJ_8.DisplayDesc.UseLight = false
	OBJ_8:SetModelState(1)
	OBJ_8.PhysicDesc.Pos.X = 458
	OBJ_8.PhysicDesc.Pos.Y = 1.95313
	OBJ_8.PhysicDesc.Pos.Z = 562
	OBJ_8.PhysicDesc.Pos.Rotation = 90
	OBJ_8:SetPhysicalActorType(1)
	OBJ_8:SetPhysicalShape(5)
	OBJ_8.PhysicDesc.Collidable = true
	OBJ_8.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_7.phy"
	OBJ_8H = ActorHandler(OBJ_8)
	environment:AddActorObject(OBJ_8H)


	OBJ_9 = ActorObjectInfo(9)
	OBJ_9:SetRenderType(1)
	OBJ_9.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_8.osgb"
	OBJ_9.DisplayDesc.UseLight = false
	OBJ_9:SetModelState(1)
	OBJ_9.PhysicDesc.Pos.X = 458
	OBJ_9.PhysicDesc.Pos.Y = 1.95313
	OBJ_9.PhysicDesc.Pos.Z = 562
	OBJ_9.PhysicDesc.Pos.Rotation = 90
	OBJ_9:SetPhysicalActorType(1)
	OBJ_9:SetPhysicalShape(5)
	OBJ_9.PhysicDesc.Collidable = true
	OBJ_9.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_8.phy"
	OBJ_9H = ActorHandler(OBJ_9)
	environment:AddActorObject(OBJ_9H)


	OBJ_10 = ActorObjectInfo(10)
	OBJ_10:SetRenderType(1)
	OBJ_10.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_9.osgb"
	OBJ_10.DisplayDesc.UseLight = false
	OBJ_10:SetModelState(1)
	OBJ_10.PhysicDesc.Pos.X = 458
	OBJ_10.PhysicDesc.Pos.Y = 1.95313
	OBJ_10.PhysicDesc.Pos.Z = 562
	OBJ_10.PhysicDesc.Pos.Rotation = 90
	OBJ_10:SetPhysicalActorType(1)
	OBJ_10:SetPhysicalShape(5)
	OBJ_10.PhysicDesc.Collidable = true
	OBJ_10.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_9.phy"
	OBJ_10H = ActorHandler(OBJ_10)
	environment:AddActorObject(OBJ_10H)


	OBJ_11 = ActorObjectInfo(11)
	OBJ_11:SetRenderType(1)
	OBJ_11.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_10.osgb"
	OBJ_11.DisplayDesc.UseLight = false
	OBJ_11:SetModelState(1)
	OBJ_11.PhysicDesc.Pos.X = 445
	OBJ_11.PhysicDesc.Pos.Y = 17.3828
	OBJ_11.PhysicDesc.Pos.Z = 578
	OBJ_11.PhysicDesc.Pos.Rotation = 90
	OBJ_11:SetPhysicalActorType(1)
	OBJ_11:SetPhysicalShape(5)
	OBJ_11.PhysicDesc.Collidable = true
	OBJ_11.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_10.phy"
	OBJ_11H = ActorHandler(OBJ_11)
	environment:AddActorObject(OBJ_11H)


	OBJ_12 = ActorObjectInfo(12)
	OBJ_12:SetRenderType(1)
	OBJ_12.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_11.osgb"
	OBJ_12.DisplayDesc.UseLight = false
	OBJ_12:SetModelState(1)
	OBJ_12.PhysicDesc.Pos.X = 445
	OBJ_12.PhysicDesc.Pos.Y = 17.3828
	OBJ_12.PhysicDesc.Pos.Z = 578
	OBJ_12.PhysicDesc.Pos.Rotation = 90
	OBJ_12:SetPhysicalActorType(1)
	OBJ_12:SetPhysicalShape(5)
	OBJ_12.PhysicDesc.Collidable = true
	OBJ_12.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_11.phy"
	OBJ_12H = ActorHandler(OBJ_12)
	environment:AddActorObject(OBJ_12H)


	OBJ_13 = ActorObjectInfo(13)
	OBJ_13:SetRenderType(1)
	OBJ_13.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_12.osgb"
	OBJ_13.DisplayDesc.UseLight = false
	OBJ_13:SetModelState(1)
	OBJ_13.PhysicDesc.Pos.X = 445
	OBJ_13.PhysicDesc.Pos.Y = 17.3828
	OBJ_13.PhysicDesc.Pos.Z = 578
	OBJ_13.PhysicDesc.Pos.Rotation = 90
	OBJ_13:SetPhysicalActorType(1)
	OBJ_13:SetPhysicalShape(5)
	OBJ_13.PhysicDesc.Collidable = true
	OBJ_13.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_12.phy"
	OBJ_13H = ActorHandler(OBJ_13)
	environment:AddActorObject(OBJ_13H)


	OBJ_14 = ActorObjectInfo(14)
	OBJ_14:SetRenderType(1)
	OBJ_14.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_13.osgb"
	OBJ_14.DisplayDesc.UseLight = false
	OBJ_14:SetModelState(1)
	OBJ_14.PhysicDesc.Pos.X = 452
	OBJ_14.PhysicDesc.Pos.Y = 23.6328
	OBJ_14.PhysicDesc.Pos.Z = 575
	OBJ_14.PhysicDesc.Pos.Rotation = 90
	OBJ_14:SetPhysicalActorType(1)
	OBJ_14:SetPhysicalShape(5)
	OBJ_14.PhysicDesc.Collidable = true
	OBJ_14.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_13.phy"
	OBJ_14H = ActorHandler(OBJ_14)
	environment:AddActorObject(OBJ_14H)


	OBJ_15 = ActorObjectInfo(15)
	OBJ_15:SetRenderType(1)
	OBJ_15.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_14.osgb"
	OBJ_15.DisplayDesc.UseLight = false
	OBJ_15:SetModelState(1)
	OBJ_15.PhysicDesc.Pos.X = 452
	OBJ_15.PhysicDesc.Pos.Y = 23.6328
	OBJ_15.PhysicDesc.Pos.Z = 575
	OBJ_15.PhysicDesc.Pos.Rotation = 90
	OBJ_15:SetPhysicalActorType(1)
	OBJ_15:SetPhysicalShape(5)
	OBJ_15.PhysicDesc.Collidable = true
	OBJ_15.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_14.phy"
	OBJ_15H = ActorHandler(OBJ_15)
	environment:AddActorObject(OBJ_15H)


	OBJ_16 = ActorObjectInfo(16)
	OBJ_16:SetRenderType(1)
	OBJ_16.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_15.osgb"
	OBJ_16.DisplayDesc.UseLight = false
	OBJ_16:SetModelState(1)
	OBJ_16.PhysicDesc.Pos.X = 447.5
	OBJ_16.PhysicDesc.Pos.Y = 7.8125
	OBJ_16.PhysicDesc.Pos.Z = 583.5
	OBJ_16.PhysicDesc.Pos.Rotation = 270
	OBJ_16:SetPhysicalActorType(1)
	OBJ_16:SetPhysicalShape(5)
	OBJ_16.PhysicDesc.Collidable = true
	OBJ_16.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_15.phy"
	OBJ_16H = ActorHandler(OBJ_16)
	environment:AddActorObject(OBJ_16H)


	OBJ_17 = ActorObjectInfo(17)
	OBJ_17:SetRenderType(1)
	OBJ_17.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_15.osgb"
	OBJ_17.DisplayDesc.UseLight = false
	OBJ_17:SetModelState(1)
	OBJ_17.PhysicDesc.Pos.X = 435.5
	OBJ_17.PhysicDesc.Pos.Y = 8.98438
	OBJ_17.PhysicDesc.Pos.Z = 578.125
	OBJ_17.PhysicDesc.Pos.Rotation = 360
	OBJ_17:SetPhysicalActorType(1)
	OBJ_17:SetPhysicalShape(5)
	OBJ_17.PhysicDesc.Collidable = true
	OBJ_17.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_15.phy"
	OBJ_17H = ActorHandler(OBJ_17)
	environment:AddActorObject(OBJ_17H)


	OBJ_18 = ActorObjectInfo(18)
	OBJ_18:SetRenderType(1)
	OBJ_18.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_16.osgb"
	OBJ_18.DisplayDesc.UseLight = false
	OBJ_18:SetModelState(1)
	OBJ_18.PhysicDesc.Pos.X = 468.5
	OBJ_18.PhysicDesc.Pos.Y = 5.66406
	OBJ_18.PhysicDesc.Pos.Z = 565.5
	OBJ_18.PhysicDesc.Pos.Rotation = 270
	OBJ_18:SetPhysicalActorType(1)
	OBJ_18:SetPhysicalShape(5)
	OBJ_18.PhysicDesc.Collidable = true
	OBJ_18.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_16.phy"
	OBJ_18H = ActorHandler(OBJ_18)
	environment:AddActorObject(OBJ_18H)


	OBJ_19 = ActorObjectInfo(19)
	OBJ_19:SetRenderType(1)
	OBJ_19.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_17.osgb"
	OBJ_19.DisplayDesc.UseLight = false
	OBJ_19:SetModelState(1)
	OBJ_19.PhysicDesc.Pos.X = 462.5
	OBJ_19.PhysicDesc.Pos.Y = 0.390625
	OBJ_19.PhysicDesc.Pos.Z = 591.5
	OBJ_19.PhysicDesc.Pos.Rotation = 270
	OBJ_19:SetPhysicalActorType(1)
	OBJ_19:SetPhysicalShape(5)
	OBJ_19.PhysicDesc.Collidable = true
	OBJ_19.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_17.phy"
	OBJ_19H = ActorHandler(OBJ_19)
	environment:AddActorObject(OBJ_19H)


	OBJ_20 = ActorObjectInfo(20)
	OBJ_20:SetRenderType(1)
	OBJ_20.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_17.osgb"
	OBJ_20.DisplayDesc.UseLight = false
	OBJ_20:SetModelState(1)
	OBJ_20.PhysicDesc.Pos.X = 465.191
	OBJ_20.PhysicDesc.Pos.Y = 0.230469
	OBJ_20.PhysicDesc.Pos.Z = 591
	OBJ_20.PhysicDesc.Pos.Rotation = 180
	OBJ_20:SetPhysicalActorType(1)
	OBJ_20:SetPhysicalShape(5)
	OBJ_20.PhysicDesc.Collidable = true
	OBJ_20.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_17.phy"
	OBJ_20H = ActorHandler(OBJ_20)
	environment:AddActorObject(OBJ_20H)


	OBJ_21 = ActorObjectInfo(21)
	OBJ_21:SetRenderType(1)
	OBJ_21.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_18.osgb"
	OBJ_21.DisplayDesc.UseLight = false
	OBJ_21:SetModelState(1)
	OBJ_21.PhysicDesc.Pos.X = 467.129
	OBJ_21.PhysicDesc.Pos.Y = 0.300781
	OBJ_21.PhysicDesc.Pos.Z = 591.625
	OBJ_21.PhysicDesc.Pos.Rotation = 360
	OBJ_21:SetPhysicalActorType(1)
	OBJ_21:SetPhysicalShape(5)
	OBJ_21.PhysicDesc.Collidable = true
	OBJ_21.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_18.phy"
	OBJ_21H = ActorHandler(OBJ_21)
	environment:AddActorObject(OBJ_21H)


	OBJ_22 = ActorObjectInfo(22)
	OBJ_22:SetRenderType(1)
	OBJ_22.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_18.osgb"
	OBJ_22.DisplayDesc.UseLight = false
	OBJ_22:SetModelState(1)
	OBJ_22.PhysicDesc.Pos.X = 466.25
	OBJ_22.PhysicDesc.Pos.Y = 0.0976563
	OBJ_22.PhysicDesc.Pos.Z = 589.25
	OBJ_22.PhysicDesc.Pos.Rotation = 90
	OBJ_22:SetPhysicalActorType(1)
	OBJ_22:SetPhysicalShape(5)
	OBJ_22.PhysicDesc.Collidable = true
	OBJ_22.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_18.phy"
	OBJ_22H = ActorHandler(OBJ_22)
	environment:AddActorObject(OBJ_22H)


	OBJ_23 = ActorObjectInfo(23)
	OBJ_23:SetRenderType(1)
	OBJ_23.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_18.osgb"
	OBJ_23.DisplayDesc.UseLight = false
	OBJ_23:SetModelState(1)
	OBJ_23.PhysicDesc.Pos.X = 463.5
	OBJ_23.PhysicDesc.Pos.Y = 0
	OBJ_23.PhysicDesc.Pos.Z = 588.5
	OBJ_23.PhysicDesc.Pos.Rotation = 180
	OBJ_23:SetPhysicalActorType(1)
	OBJ_23:SetPhysicalShape(5)
	OBJ_23.PhysicDesc.Collidable = true
	OBJ_23.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_18.phy"
	OBJ_23H = ActorHandler(OBJ_23)
	environment:AddActorObject(OBJ_23H)


	OBJ_24 = ActorObjectInfo(24)
	OBJ_24:SetRenderType(1)
	OBJ_24.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_19.osgb"
	OBJ_24.DisplayDesc.UseLight = false
	OBJ_24:SetModelState(1)
	OBJ_24.PhysicDesc.Pos.X = 466.75
	OBJ_24.PhysicDesc.Pos.Y = 0.296875
	OBJ_24.PhysicDesc.Pos.Z = 590.5
	OBJ_24.PhysicDesc.Pos.Rotation = 360
	OBJ_24:SetPhysicalActorType(1)
	OBJ_24:SetPhysicalShape(5)
	OBJ_24.PhysicDesc.Collidable = true
	OBJ_24.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_19.phy"
	OBJ_24H = ActorHandler(OBJ_24)
	environment:AddActorObject(OBJ_24H)


	OBJ_25 = ActorObjectInfo(25)
	OBJ_25:SetRenderType(1)
	OBJ_25.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_19.osgb"
	OBJ_25.DisplayDesc.UseLight = false
	OBJ_25:SetModelState(1)
	OBJ_25.PhysicDesc.Pos.X = 465.125
	OBJ_25.PhysicDesc.Pos.Y = 0
	OBJ_25.PhysicDesc.Pos.Z = 588.625
	OBJ_25.PhysicDesc.Pos.Rotation = 90
	OBJ_25:SetPhysicalActorType(1)
	OBJ_25:SetPhysicalShape(5)
	OBJ_25.PhysicDesc.Collidable = true
	OBJ_25.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_19.phy"
	OBJ_25H = ActorHandler(OBJ_25)
	environment:AddActorObject(OBJ_25H)


	OBJ_26 = ActorObjectInfo(26)
	OBJ_26:SetRenderType(1)
	OBJ_26.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_18.osgb"
	OBJ_26.DisplayDesc.UseLight = false
	OBJ_26:SetModelState(1)
	OBJ_26.PhysicDesc.Pos.X = 464.5
	OBJ_26.PhysicDesc.Pos.Y = 0
	OBJ_26.PhysicDesc.Pos.Z = 592.5
	OBJ_26.PhysicDesc.Pos.Rotation = 90
	OBJ_26:SetPhysicalActorType(1)
	OBJ_26:SetPhysicalShape(5)
	OBJ_26.PhysicDesc.Collidable = true
	OBJ_26.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_18.phy"
	OBJ_26H = ActorHandler(OBJ_26)
	environment:AddActorObject(OBJ_26H)


	OBJ_27 = ActorObjectInfo(27)
	OBJ_27:SetRenderType(1)
	OBJ_27.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_20.osgb"
	OBJ_27.DisplayDesc.UseLight = false
	OBJ_27:SetModelState(1)
	OBJ_27.PhysicDesc.Pos.X = 435.5
	OBJ_27.PhysicDesc.Pos.Y = 11.5234
	OBJ_27.PhysicDesc.Pos.Z = 512.5
	OBJ_27.PhysicDesc.Pos.Rotation = 270
	OBJ_27:SetPhysicalActorType(1)
	OBJ_27:SetPhysicalShape(5)
	OBJ_27.PhysicDesc.Collidable = true
	OBJ_27.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_20.phy"
	OBJ_27H = ActorHandler(OBJ_27)
	environment:AddActorObject(OBJ_27H)


	OBJ_28 = ActorObjectInfo(28)
	OBJ_28:SetRenderType(1)
	OBJ_28.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_21.osgb"
	OBJ_28.DisplayDesc.UseLight = false
	OBJ_28:SetModelState(1)
	OBJ_28.PhysicDesc.Pos.X = 435.5
	OBJ_28.PhysicDesc.Pos.Y = 11.5234
	OBJ_28.PhysicDesc.Pos.Z = 512.5
	OBJ_28.PhysicDesc.Pos.Rotation = 90
	OBJ_28:SetPhysicalActorType(1)
	OBJ_28:SetPhysicalShape(5)
	OBJ_28.PhysicDesc.Collidable = true
	OBJ_28.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_21.phy"
	OBJ_28H = ActorHandler(OBJ_28)
	environment:AddActorObject(OBJ_28H)


	OBJ_29 = ActorObjectInfo(29)
	OBJ_29:SetRenderType(1)
	OBJ_29.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_22.osgb"
	OBJ_29.DisplayDesc.UseLight = false
	OBJ_29:SetModelState(1)
	OBJ_29.PhysicDesc.Pos.X = 435.5
	OBJ_29.PhysicDesc.Pos.Y = 11.5234
	OBJ_29.PhysicDesc.Pos.Z = 512.5
	OBJ_29.PhysicDesc.Pos.Rotation = 270
	OBJ_29:SetPhysicalActorType(1)
	OBJ_29:SetPhysicalShape(5)
	OBJ_29.PhysicDesc.Collidable = true
	OBJ_29.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_22.phy"
	OBJ_29H = ActorHandler(OBJ_29)
	environment:AddActorObject(OBJ_29H)


	OBJ_30 = ActorObjectInfo(30)
	OBJ_30:SetRenderType(1)
	OBJ_30.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_23.osgb"
	OBJ_30.DisplayDesc.UseLight = false
	OBJ_30:SetModelState(1)
	OBJ_30.PhysicDesc.Pos.X = 437.5
	OBJ_30.PhysicDesc.Pos.Y = 11.5234
	OBJ_30.PhysicDesc.Pos.Z = 518.125
	OBJ_30.PhysicDesc.Pos.Rotation = 360
	OBJ_30:SetPhysicalActorType(1)
	OBJ_30:SetPhysicalShape(5)
	OBJ_30.PhysicDesc.Collidable = true
	OBJ_30.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_23.phy"
	OBJ_30H = ActorHandler(OBJ_30)
	environment:AddActorObject(OBJ_30H)


	OBJ_31 = ActorObjectInfo(31)
	OBJ_31:SetRenderType(1)
	OBJ_31.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_23.osgb"
	OBJ_31.DisplayDesc.UseLight = false
	OBJ_31:SetModelState(1)
	OBJ_31.PhysicDesc.Pos.X = 433.25
	OBJ_31.PhysicDesc.Pos.Y = 11.5234
	OBJ_31.PhysicDesc.Pos.Z = 518.125
	OBJ_31.PhysicDesc.Pos.Rotation = 360
	OBJ_31:SetPhysicalActorType(1)
	OBJ_31:SetPhysicalShape(5)
	OBJ_31.PhysicDesc.Collidable = true
	OBJ_31.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_23.phy"
	OBJ_31H = ActorHandler(OBJ_31)
	environment:AddActorObject(OBJ_31H)


	OBJ_32 = ActorObjectInfo(32)
	OBJ_32:SetRenderType(1)
	OBJ_32.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_23.osgb"
	OBJ_32.DisplayDesc.UseLight = false
	OBJ_32:SetModelState(1)
	OBJ_32.PhysicDesc.Pos.X = 433.25
	OBJ_32.PhysicDesc.Pos.Y = 11.5234
	OBJ_32.PhysicDesc.Pos.Z = 507
	OBJ_32.PhysicDesc.Pos.Rotation = 180
	OBJ_32:SetPhysicalActorType(1)
	OBJ_32:SetPhysicalShape(5)
	OBJ_32.PhysicDesc.Collidable = true
	OBJ_32.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_23.phy"
	OBJ_32H = ActorHandler(OBJ_32)
	environment:AddActorObject(OBJ_32H)


	OBJ_33 = ActorObjectInfo(33)
	OBJ_33:SetRenderType(1)
	OBJ_33.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_23.osgb"
	OBJ_33.DisplayDesc.UseLight = false
	OBJ_33:SetModelState(1)
	OBJ_33.PhysicDesc.Pos.X = 437.5
	OBJ_33.PhysicDesc.Pos.Y = 11.5234
	OBJ_33.PhysicDesc.Pos.Z = 507
	OBJ_33.PhysicDesc.Pos.Rotation = 180
	OBJ_33:SetPhysicalActorType(1)
	OBJ_33:SetPhysicalShape(5)
	OBJ_33.PhysicDesc.Collidable = true
	OBJ_33.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_23.phy"
	OBJ_33H = ActorHandler(OBJ_33)
	environment:AddActorObject(OBJ_33H)


	OBJ_34 = ActorObjectInfo(34)
	OBJ_34:SetRenderType(1)
	OBJ_34.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_23.osgb"
	OBJ_34.DisplayDesc.UseLight = false
	OBJ_34:SetModelState(1)
	OBJ_34.PhysicDesc.Pos.X = 430.75
	OBJ_34.PhysicDesc.Pos.Y = 11.5234
	OBJ_34.PhysicDesc.Pos.Z = 514.75
	OBJ_34.PhysicDesc.Pos.Rotation = 270
	OBJ_34:SetPhysicalActorType(1)
	OBJ_34:SetPhysicalShape(5)
	OBJ_34.PhysicDesc.Collidable = true
	OBJ_34.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_23.phy"
	OBJ_34H = ActorHandler(OBJ_34)
	environment:AddActorObject(OBJ_34H)


	OBJ_35 = ActorObjectInfo(35)
	OBJ_35:SetRenderType(1)
	OBJ_35.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_23.osgb"
	OBJ_35.DisplayDesc.UseLight = false
	OBJ_35:SetModelState(1)
	OBJ_35.PhysicDesc.Pos.X = 430.75
	OBJ_35.PhysicDesc.Pos.Y = 11.5234
	OBJ_35.PhysicDesc.Pos.Z = 510.25
	OBJ_35.PhysicDesc.Pos.Rotation = 270
	OBJ_35:SetPhysicalActorType(1)
	OBJ_35:SetPhysicalShape(5)
	OBJ_35.PhysicDesc.Collidable = true
	OBJ_35.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_23.phy"
	OBJ_35H = ActorHandler(OBJ_35)
	environment:AddActorObject(OBJ_35H)


	OBJ_36 = ActorObjectInfo(36)
	OBJ_36:SetRenderType(1)
	OBJ_36.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_24.osgb"
	OBJ_36.DisplayDesc.UseLight = false
	OBJ_36:SetModelState(1)
	OBJ_36.PhysicDesc.Pos.X = 448.098
	OBJ_36.PhysicDesc.Pos.Y = 11.5234
	OBJ_36.PhysicDesc.Pos.Z = 514.93
	OBJ_36.PhysicDesc.Pos.Rotation = 90
	OBJ_36:SetPhysicalActorType(1)
	OBJ_36:SetPhysicalShape(5)
	OBJ_36.PhysicDesc.Collidable = true
	OBJ_36.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_24.phy"
	OBJ_36H = ActorHandler(OBJ_36)
	environment:AddActorObject(OBJ_36H)


	OBJ_37 = ActorObjectInfo(37)
	OBJ_37:SetRenderType(1)
	OBJ_37.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_24.osgb"
	OBJ_37.DisplayDesc.UseLight = false
	OBJ_37:SetModelState(1)
	OBJ_37.PhysicDesc.Pos.X = 448.098
	OBJ_37.PhysicDesc.Pos.Y = 11.5234
	OBJ_37.PhysicDesc.Pos.Z = 510.189
	OBJ_37.PhysicDesc.Pos.Rotation = 90
	OBJ_37:SetPhysicalActorType(1)
	OBJ_37:SetPhysicalShape(5)
	OBJ_37.PhysicDesc.Collidable = true
	OBJ_37.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_24.phy"
	OBJ_37H = ActorHandler(OBJ_37)
	environment:AddActorObject(OBJ_37H)


	OBJ_38 = ActorObjectInfo(38)
	OBJ_38:SetRenderType(1)
	OBJ_38.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_23.osgb"
	OBJ_38.DisplayDesc.UseLight = false
	OBJ_38:SetModelState(1)
	OBJ_38.PhysicDesc.Pos.X = 445.5
	OBJ_38.PhysicDesc.Pos.Y = 11.5234
	OBJ_38.PhysicDesc.Pos.Z = 518.625
	OBJ_38.PhysicDesc.Pos.Rotation = 360
	OBJ_38:SetPhysicalActorType(1)
	OBJ_38:SetPhysicalShape(5)
	OBJ_38.PhysicDesc.Collidable = true
	OBJ_38.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_23.phy"
	OBJ_38H = ActorHandler(OBJ_38)
	environment:AddActorObject(OBJ_38H)


	OBJ_39 = ActorObjectInfo(39)
	OBJ_39:SetRenderType(1)
	OBJ_39.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_23.osgb"
	OBJ_39.DisplayDesc.UseLight = false
	OBJ_39:SetModelState(1)
	OBJ_39.PhysicDesc.Pos.X = 445.5
	OBJ_39.PhysicDesc.Pos.Y = 11.5234
	OBJ_39.PhysicDesc.Pos.Z = 506.5
	OBJ_39.PhysicDesc.Pos.Rotation = 180
	OBJ_39:SetPhysicalActorType(1)
	OBJ_39:SetPhysicalShape(5)
	OBJ_39.PhysicDesc.Collidable = true
	OBJ_39.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_23.phy"
	OBJ_39H = ActorHandler(OBJ_39)
	environment:AddActorObject(OBJ_39H)


	OBJ_40 = ActorObjectInfo(40)
	OBJ_40:SetRenderType(1)
	OBJ_40.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_25.osgb"
	OBJ_40.DisplayDesc.UseLight = false
	OBJ_40:SetModelState(1)
	OBJ_40.PhysicDesc.Pos.X = 454.5
	OBJ_40.PhysicDesc.Pos.Y = 10.1563
	OBJ_40.PhysicDesc.Pos.Z = 524.5
	OBJ_40.PhysicDesc.Pos.Rotation = 270
	OBJ_40:SetPhysicalActorType(1)
	OBJ_40:SetPhysicalShape(5)
	OBJ_40.PhysicDesc.Collidable = true
	OBJ_40.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_25.phy"
	OBJ_40H = ActorHandler(OBJ_40)
	environment:AddActorObject(OBJ_40H)


	OBJ_41 = ActorObjectInfo(41)
	OBJ_41:SetRenderType(1)
	OBJ_41.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_25.osgb"
	OBJ_41.DisplayDesc.UseLight = false
	OBJ_41:SetModelState(1)
	OBJ_41.PhysicDesc.Pos.X = 454.5
	OBJ_41.PhysicDesc.Pos.Y = 10.1563
	OBJ_41.PhysicDesc.Pos.Z = 500.5
	OBJ_41.PhysicDesc.Pos.Rotation = 270
	OBJ_41:SetPhysicalActorType(1)
	OBJ_41:SetPhysicalShape(5)
	OBJ_41.PhysicDesc.Collidable = true
	OBJ_41.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_25.phy"
	OBJ_41H = ActorHandler(OBJ_41)
	environment:AddActorObject(OBJ_41H)


	OBJ_42 = ActorObjectInfo(42)
	OBJ_42:SetRenderType(1)
	OBJ_42.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_25.osgb"
	OBJ_42.DisplayDesc.UseLight = false
	OBJ_42:SetModelState(1)
	OBJ_42.PhysicDesc.Pos.X = 424.5
	OBJ_42.PhysicDesc.Pos.Y = 10.1563
	OBJ_42.PhysicDesc.Pos.Z = 500.5
	OBJ_42.PhysicDesc.Pos.Rotation = 270
	OBJ_42:SetPhysicalActorType(1)
	OBJ_42:SetPhysicalShape(5)
	OBJ_42.PhysicDesc.Collidable = true
	OBJ_42.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_25.phy"
	OBJ_42H = ActorHandler(OBJ_42)
	environment:AddActorObject(OBJ_42H)


	OBJ_43 = ActorObjectInfo(43)
	OBJ_43:SetRenderType(1)
	OBJ_43.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_25.osgb"
	OBJ_43.DisplayDesc.UseLight = false
	OBJ_43:SetModelState(1)
	OBJ_43.PhysicDesc.Pos.X = 424.5
	OBJ_43.PhysicDesc.Pos.Y = 10.1563
	OBJ_43.PhysicDesc.Pos.Z = 524.5
	OBJ_43.PhysicDesc.Pos.Rotation = 270
	OBJ_43:SetPhysicalActorType(1)
	OBJ_43:SetPhysicalShape(5)
	OBJ_43.PhysicDesc.Collidable = true
	OBJ_43.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_25.phy"
	OBJ_43H = ActorHandler(OBJ_43)
	environment:AddActorObject(OBJ_43H)


	OBJ_44 = ActorObjectInfo(44)
	OBJ_44:SetRenderType(1)
	OBJ_44.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_26.osgb"
	OBJ_44.DisplayDesc.UseLight = false
	OBJ_44:SetModelState(1)
	OBJ_44.PhysicDesc.Pos.X = 463.5
	OBJ_44.PhysicDesc.Pos.Y = 7.8125
	OBJ_44.PhysicDesc.Pos.Z = 514.5
	OBJ_44.PhysicDesc.Pos.Rotation = 180
	OBJ_44:SetPhysicalActorType(1)
	OBJ_44:SetPhysicalShape(5)
	OBJ_44.PhysicDesc.Collidable = true
	OBJ_44.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_26.phy"
	OBJ_44H = ActorHandler(OBJ_44)
	environment:AddActorObject(OBJ_44H)


	OBJ_45 = ActorObjectInfo(45)
	OBJ_45:SetRenderType(1)
	OBJ_45.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_26.osgb"
	OBJ_45.DisplayDesc.UseLight = false
	OBJ_45:SetModelState(1)
	OBJ_45.PhysicDesc.Pos.X = 463.5
	OBJ_45.PhysicDesc.Pos.Y = 7.8125
	OBJ_45.PhysicDesc.Pos.Z = 510.5
	OBJ_45.PhysicDesc.Pos.Rotation = 180
	OBJ_45:SetPhysicalActorType(1)
	OBJ_45:SetPhysicalShape(5)
	OBJ_45.PhysicDesc.Collidable = true
	OBJ_45.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_26.phy"
	OBJ_45H = ActorHandler(OBJ_45)
	environment:AddActorObject(OBJ_45H)


	OBJ_46 = ActorObjectInfo(46)
	OBJ_46:SetRenderType(1)
	OBJ_46.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_26.osgb"
	OBJ_46.DisplayDesc.UseLight = false
	OBJ_46:SetModelState(1)
	OBJ_46.PhysicDesc.Pos.X = 474.5
	OBJ_46.PhysicDesc.Pos.Y = 7.8125
	OBJ_46.PhysicDesc.Pos.Z = 514.5
	OBJ_46.PhysicDesc.Pos.Rotation = 180
	OBJ_46:SetPhysicalActorType(1)
	OBJ_46:SetPhysicalShape(5)
	OBJ_46.PhysicDesc.Collidable = true
	OBJ_46.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_26.phy"
	OBJ_46H = ActorHandler(OBJ_46)
	environment:AddActorObject(OBJ_46H)


	OBJ_47 = ActorObjectInfo(47)
	OBJ_47:SetRenderType(1)
	OBJ_47.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_26.osgb"
	OBJ_47.DisplayDesc.UseLight = false
	OBJ_47:SetModelState(1)
	OBJ_47.PhysicDesc.Pos.X = 474.5
	OBJ_47.PhysicDesc.Pos.Y = 7.8125
	OBJ_47.PhysicDesc.Pos.Z = 510.5
	OBJ_47.PhysicDesc.Pos.Rotation = 180
	OBJ_47:SetPhysicalActorType(1)
	OBJ_47:SetPhysicalShape(5)
	OBJ_47.PhysicDesc.Collidable = true
	OBJ_47.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_26.phy"
	OBJ_47H = ActorHandler(OBJ_47)
	environment:AddActorObject(OBJ_47H)


	OBJ_48 = ActorObjectInfo(48)
	OBJ_48:SetRenderType(1)
	OBJ_48.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_27.osgb"
	OBJ_48.DisplayDesc.UseLight = false
	OBJ_48:SetModelState(1)
	OBJ_48.PhysicDesc.Pos.X = 441.25
	OBJ_48.PhysicDesc.Pos.Y = 11.5234
	OBJ_48.PhysicDesc.Pos.Z = 518.625
	OBJ_48.PhysicDesc.Pos.Rotation = 360
	OBJ_48:SetPhysicalActorType(1)
	OBJ_48:SetPhysicalShape(5)
	OBJ_48.PhysicDesc.Collidable = true
	OBJ_48.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_27.phy"
	OBJ_48H = ActorHandler(OBJ_48)
	environment:AddActorObject(OBJ_48H)


	OBJ_49 = ActorObjectInfo(49)
	OBJ_49:SetRenderType(1)
	OBJ_49.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_27.osgb"
	OBJ_49.DisplayDesc.UseLight = false
	OBJ_49:SetModelState(1)
	OBJ_49.PhysicDesc.Pos.X = 441.25
	OBJ_49.PhysicDesc.Pos.Y = 11.5234
	OBJ_49.PhysicDesc.Pos.Z = 506.5
	OBJ_49.PhysicDesc.Pos.Rotation = 180
	OBJ_49:SetPhysicalActorType(1)
	OBJ_49:SetPhysicalShape(5)
	OBJ_49.PhysicDesc.Collidable = true
	OBJ_49.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_27.phy"
	OBJ_49H = ActorHandler(OBJ_49)
	environment:AddActorObject(OBJ_49H)


	OBJ_50 = ActorObjectInfo(50)
	OBJ_50:SetRenderType(1)
	OBJ_50.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_28.osgb"
	OBJ_50.DisplayDesc.UseLight = false
	OBJ_50:SetModelState(1)
	OBJ_50.PhysicDesc.Pos.X = 435.5
	OBJ_50.PhysicDesc.Pos.Y = 11.5234
	OBJ_50.PhysicDesc.Pos.Z = 512.5
	OBJ_50.PhysicDesc.Pos.Rotation = 270
	OBJ_50:SetPhysicalActorType(1)
	OBJ_50:SetPhysicalShape(5)
	OBJ_50.PhysicDesc.Collidable = true
	OBJ_50.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_28.phy"
	OBJ_50H = ActorHandler(OBJ_50)
	environment:AddActorObject(OBJ_50H)


	OBJ_51 = ActorObjectInfo(51)
	OBJ_51:SetRenderType(1)
	OBJ_51.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_29.osgb"
	OBJ_51.DisplayDesc.UseLight = false
	OBJ_51:SetModelState(1)
	OBJ_51.PhysicDesc.Pos.X = 435.5
	OBJ_51.PhysicDesc.Pos.Y = 11.5234
	OBJ_51.PhysicDesc.Pos.Z = 512.5
	OBJ_51.PhysicDesc.Pos.Rotation = 270
	OBJ_51:SetPhysicalActorType(1)
	OBJ_51:SetPhysicalShape(5)
	OBJ_51.PhysicDesc.Collidable = true
	OBJ_51.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_29.phy"
	OBJ_51H = ActorHandler(OBJ_51)
	environment:AddActorObject(OBJ_51H)


	OBJ_52 = ActorObjectInfo(52)
	OBJ_52:SetRenderType(1)
	OBJ_52.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_30.osgb"
	OBJ_52.DisplayDesc.UseLight = false
	OBJ_52:SetModelState(1)
	OBJ_52.PhysicDesc.Pos.X = 435.5
	OBJ_52.PhysicDesc.Pos.Y = 11.5234
	OBJ_52.PhysicDesc.Pos.Z = 512.5
	OBJ_52.PhysicDesc.Pos.Rotation = 270
	OBJ_52:SetPhysicalActorType(1)
	OBJ_52:SetPhysicalShape(5)
	OBJ_52.PhysicDesc.Collidable = true
	OBJ_52.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_30.phy"
	OBJ_52H = ActorHandler(OBJ_52)
	environment:AddActorObject(OBJ_52H)


	OBJ_53 = ActorObjectInfo(53)
	OBJ_53:SetRenderType(1)
	OBJ_53.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_31.osgb"
	OBJ_53.DisplayDesc.UseLight = false
	OBJ_53:SetModelState(1)
	OBJ_53.PhysicDesc.Pos.X = 435.5
	OBJ_53.PhysicDesc.Pos.Y = 11.5234
	OBJ_53.PhysicDesc.Pos.Z = 512.5
	OBJ_53.PhysicDesc.Pos.Rotation = 270
	OBJ_53:SetPhysicalActorType(1)
	OBJ_53:SetPhysicalShape(5)
	OBJ_53.PhysicDesc.Collidable = true
	OBJ_53.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_31.phy"
	OBJ_53H = ActorHandler(OBJ_53)
	environment:AddActorObject(OBJ_53H)


	OBJ_54 = ActorObjectInfo(54)
	OBJ_54:SetRenderType(1)
	OBJ_54.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_32.osgb"
	OBJ_54.DisplayDesc.UseLight = false
	OBJ_54:SetModelState(1)
	OBJ_54.PhysicDesc.Pos.X = 435.5
	OBJ_54.PhysicDesc.Pos.Y = 11.5234
	OBJ_54.PhysicDesc.Pos.Z = 512.5
	OBJ_54.PhysicDesc.Pos.Rotation = 270
	OBJ_54:SetPhysicalActorType(1)
	OBJ_54:SetPhysicalShape(5)
	OBJ_54.PhysicDesc.Collidable = true
	OBJ_54.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_32.phy"
	OBJ_54H = ActorHandler(OBJ_54)
	environment:AddActorObject(OBJ_54H)


	OBJ_55 = ActorObjectInfo(55)
	OBJ_55:SetRenderType(1)
	OBJ_55.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_33.osgb"
	OBJ_55.DisplayDesc.UseLight = false
	OBJ_55:SetModelState(1)
	OBJ_55.PhysicDesc.Pos.X = 455.5
	OBJ_55.PhysicDesc.Pos.Y = 10.1563
	OBJ_55.PhysicDesc.Pos.Z = 521.5
	OBJ_55.PhysicDesc.Pos.Rotation = 270
	OBJ_55:SetPhysicalActorType(1)
	OBJ_55:SetPhysicalShape(5)
	OBJ_55.PhysicDesc.Collidable = true
	OBJ_55.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_33.phy"
	OBJ_55H = ActorHandler(OBJ_55)
	environment:AddActorObject(OBJ_55H)


	OBJ_56 = ActorObjectInfo(56)
	OBJ_56:SetRenderType(1)
	OBJ_56.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_34.osgb"
	OBJ_56.DisplayDesc.UseLight = false
	OBJ_56:SetModelState(1)
	OBJ_56.PhysicDesc.Pos.X = 450.5
	OBJ_56.PhysicDesc.Pos.Y = 11.5234
	OBJ_56.PhysicDesc.Pos.Z = 519.5
	OBJ_56.PhysicDesc.Pos.Rotation = 270
	OBJ_56:SetPhysicalActorType(1)
	OBJ_56:SetPhysicalShape(5)
	OBJ_56.PhysicDesc.Collidable = true
	OBJ_56.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_34.phy"
	OBJ_56H = ActorHandler(OBJ_56)
	environment:AddActorObject(OBJ_56H)


	OBJ_57 = ActorObjectInfo(57)
	OBJ_57:SetRenderType(1)
	OBJ_57.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_34.osgb"
	OBJ_57.DisplayDesc.UseLight = false
	OBJ_57:SetModelState(1)
	OBJ_57.PhysicDesc.Pos.X = 450.5
	OBJ_57.PhysicDesc.Pos.Y = 11.5234
	OBJ_57.PhysicDesc.Pos.Z = 505.5
	OBJ_57.PhysicDesc.Pos.Rotation = 90
	OBJ_57:SetPhysicalActorType(1)
	OBJ_57:SetPhysicalShape(5)
	OBJ_57.PhysicDesc.Collidable = true
	OBJ_57.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_34.phy"
	OBJ_57H = ActorHandler(OBJ_57)
	environment:AddActorObject(OBJ_57H)


	OBJ_58 = ActorObjectInfo(58)
	OBJ_58:SetRenderType(1)
	OBJ_58.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_35.osgb"
	OBJ_58.DisplayDesc.UseLight = false
	OBJ_58:SetModelState(1)
	OBJ_58.PhysicDesc.Pos.X = 511
	OBJ_58.PhysicDesc.Pos.Y = 0
	OBJ_58.PhysicDesc.Pos.Z = 580
	OBJ_58.PhysicDesc.Pos.Rotation = 90
	OBJ_58:SetPhysicalActorType(1)
	OBJ_58:SetPhysicalShape(5)
	OBJ_58.PhysicDesc.Collidable = true
	OBJ_58.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_35.phy"
	OBJ_58H = ActorHandler(OBJ_58)
	environment:AddActorObject(OBJ_58H)


	OBJ_59 = ActorObjectInfo(59)
	OBJ_59:SetRenderType(1)
	OBJ_59.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_36.osgb"
	OBJ_59.DisplayDesc.UseLight = false
	OBJ_59:SetModelState(1)
	OBJ_59.PhysicDesc.Pos.X = 511
	OBJ_59.PhysicDesc.Pos.Y = 0
	OBJ_59.PhysicDesc.Pos.Z = 580
	OBJ_59.PhysicDesc.Pos.Rotation = 90
	OBJ_59:SetPhysicalActorType(1)
	OBJ_59:SetPhysicalShape(5)
	OBJ_59.PhysicDesc.Collidable = true
	OBJ_59.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_36.phy"
	OBJ_59H = ActorHandler(OBJ_59)
	environment:AddActorObject(OBJ_59H)


	OBJ_60 = ActorObjectInfo(60)
	OBJ_60:SetRenderType(1)
	OBJ_60.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_37.osgb"
	OBJ_60.DisplayDesc.UseLight = false
	OBJ_60:SetModelState(1)
	OBJ_60.PhysicDesc.Pos.X = 511
	OBJ_60.PhysicDesc.Pos.Y = 0
	OBJ_60.PhysicDesc.Pos.Z = 580
	OBJ_60.PhysicDesc.Pos.Rotation = 90
	OBJ_60:SetPhysicalActorType(1)
	OBJ_60:SetPhysicalShape(5)
	OBJ_60.PhysicDesc.Collidable = true
	OBJ_60.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_37.phy"
	OBJ_60H = ActorHandler(OBJ_60)
	environment:AddActorObject(OBJ_60H)


	OBJ_61 = ActorObjectInfo(61)
	OBJ_61:SetRenderType(1)
	OBJ_61.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_38.osgb"
	OBJ_61.DisplayDesc.UseLight = false
	OBJ_61:SetModelState(1)
	OBJ_61.PhysicDesc.Pos.X = 511
	OBJ_61.PhysicDesc.Pos.Y = 0
	OBJ_61.PhysicDesc.Pos.Z = 580
	OBJ_61.PhysicDesc.Pos.Rotation = 90
	OBJ_61:SetPhysicalActorType(1)
	OBJ_61:SetPhysicalShape(5)
	OBJ_61.PhysicDesc.Collidable = true
	OBJ_61.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_38.phy"
	OBJ_61H = ActorHandler(OBJ_61)
	environment:AddActorObject(OBJ_61H)


	OBJ_62 = ActorObjectInfo(62)
	OBJ_62:SetRenderType(1)
	OBJ_62.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_39.osgb"
	OBJ_62.DisplayDesc.UseLight = false
	OBJ_62:SetModelState(1)
	OBJ_62.PhysicDesc.Pos.X = 511
	OBJ_62.PhysicDesc.Pos.Y = 0
	OBJ_62.PhysicDesc.Pos.Z = 580
	OBJ_62.PhysicDesc.Pos.Rotation = 90
	OBJ_62:SetPhysicalActorType(1)
	OBJ_62:SetPhysicalShape(5)
	OBJ_62.PhysicDesc.Collidable = true
	OBJ_62.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_39.phy"
	OBJ_62H = ActorHandler(OBJ_62)
	environment:AddActorObject(OBJ_62H)


	OBJ_63 = ActorObjectInfo(63)
	OBJ_63:SetRenderType(1)
	OBJ_63.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_40.osgb"
	OBJ_63.DisplayDesc.UseLight = false
	OBJ_63:SetModelState(1)
	OBJ_63.PhysicDesc.Pos.X = 511
	OBJ_63.PhysicDesc.Pos.Y = 0
	OBJ_63.PhysicDesc.Pos.Z = 580
	OBJ_63.PhysicDesc.Pos.Rotation = 90
	OBJ_63:SetPhysicalActorType(1)
	OBJ_63:SetPhysicalShape(5)
	OBJ_63.PhysicDesc.Collidable = true
	OBJ_63.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_40.phy"
	OBJ_63H = ActorHandler(OBJ_63)
	environment:AddActorObject(OBJ_63H)


	OBJ_64 = ActorObjectInfo(64)
	OBJ_64:SetRenderType(1)
	OBJ_64.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_41.osgb"
	OBJ_64.DisplayDesc.UseLight = false
	OBJ_64:SetModelState(1)
	OBJ_64.PhysicDesc.Pos.X = 511
	OBJ_64.PhysicDesc.Pos.Y = 0
	OBJ_64.PhysicDesc.Pos.Z = 580
	OBJ_64.PhysicDesc.Pos.Rotation = 90
	OBJ_64:SetPhysicalActorType(1)
	OBJ_64:SetPhysicalShape(5)
	OBJ_64.PhysicDesc.Collidable = true
	OBJ_64.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_41.phy"
	OBJ_64H = ActorHandler(OBJ_64)
	environment:AddActorObject(OBJ_64H)


	OBJ_65 = ActorObjectInfo(65)
	OBJ_65:SetRenderType(1)
	OBJ_65.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_42.osgb"
	OBJ_65.DisplayDesc.UseLight = false
	OBJ_65:SetModelState(1)
	OBJ_65.PhysicDesc.Pos.X = 520.5
	OBJ_65.PhysicDesc.Pos.Y = 0
	OBJ_65.PhysicDesc.Pos.Z = 566.5
	OBJ_65.PhysicDesc.Pos.Rotation = 270
	OBJ_65:SetPhysicalActorType(1)
	OBJ_65:SetPhysicalShape(5)
	OBJ_65.PhysicDesc.Collidable = true
	OBJ_65.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_42.phy"
	OBJ_65H = ActorHandler(OBJ_65)
	environment:AddActorObject(OBJ_65H)


	OBJ_66 = ActorObjectInfo(66)
	OBJ_66:SetRenderType(1)
	OBJ_66.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_42.osgb"
	OBJ_66.DisplayDesc.UseLight = false
	OBJ_66:SetModelState(1)
	OBJ_66.PhysicDesc.Pos.X = 525.5
	OBJ_66.PhysicDesc.Pos.Y = 0
	OBJ_66.PhysicDesc.Pos.Z = 565.5
	OBJ_66.PhysicDesc.Pos.Rotation = 270
	OBJ_66:SetPhysicalActorType(1)
	OBJ_66:SetPhysicalShape(5)
	OBJ_66.PhysicDesc.Collidable = true
	OBJ_66.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_42.phy"
	OBJ_66H = ActorHandler(OBJ_66)
	environment:AddActorObject(OBJ_66H)


	OBJ_67 = ActorObjectInfo(67)
	OBJ_67:SetRenderType(1)
	OBJ_67.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_42.osgb"
	OBJ_67.DisplayDesc.UseLight = false
	OBJ_67:SetModelState(1)
	OBJ_67.PhysicDesc.Pos.X = 523.5
	OBJ_67.PhysicDesc.Pos.Y = 0
	OBJ_67.PhysicDesc.Pos.Z = 568.5
	OBJ_67.PhysicDesc.Pos.Rotation = 270
	OBJ_67:SetPhysicalActorType(1)
	OBJ_67:SetPhysicalShape(5)
	OBJ_67.PhysicDesc.Collidable = true
	OBJ_67.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_42.phy"
	OBJ_67H = ActorHandler(OBJ_67)
	environment:AddActorObject(OBJ_67H)


	OBJ_68 = ActorObjectInfo(68)
	OBJ_68:SetRenderType(1)
	OBJ_68.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_43.osgb"
	OBJ_68.DisplayDesc.UseLight = false
	OBJ_68:SetModelState(1)
	OBJ_68.PhysicDesc.Pos.X = 522.5
	OBJ_68.PhysicDesc.Pos.Y = 0
	OBJ_68.PhysicDesc.Pos.Z = 564.5
	OBJ_68.PhysicDesc.Pos.Rotation = 270
	OBJ_68:SetPhysicalActorType(1)
	OBJ_68:SetPhysicalShape(5)
	OBJ_68.PhysicDesc.Collidable = true
	OBJ_68.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_43.phy"
	OBJ_68H = ActorHandler(OBJ_68)
	environment:AddActorObject(OBJ_68H)


	OBJ_69 = ActorObjectInfo(69)
	OBJ_69:SetRenderType(1)
	OBJ_69.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_43.osgb"
	OBJ_69.DisplayDesc.UseLight = false
	OBJ_69:SetModelState(1)
	OBJ_69.PhysicDesc.Pos.X = 524.5
	OBJ_69.PhysicDesc.Pos.Y = 0
	OBJ_69.PhysicDesc.Pos.Z = 566.5
	OBJ_69.PhysicDesc.Pos.Rotation = 180
	OBJ_69:SetPhysicalActorType(1)
	OBJ_69:SetPhysicalShape(5)
	OBJ_69.PhysicDesc.Collidable = true
	OBJ_69.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_43.phy"
	OBJ_69H = ActorHandler(OBJ_69)
	environment:AddActorObject(OBJ_69H)


	OBJ_70 = ActorObjectInfo(70)
	OBJ_70:SetRenderType(1)
	OBJ_70.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_43.osgb"
	OBJ_70.DisplayDesc.UseLight = false
	OBJ_70:SetModelState(1)
	OBJ_70.PhysicDesc.Pos.X = 526.5
	OBJ_70.PhysicDesc.Pos.Y = 0
	OBJ_70.PhysicDesc.Pos.Z = 564.5
	OBJ_70.PhysicDesc.Pos.Rotation = 180
	OBJ_70:SetPhysicalActorType(1)
	OBJ_70:SetPhysicalShape(5)
	OBJ_70.PhysicDesc.Collidable = true
	OBJ_70.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_43.phy"
	OBJ_70H = ActorHandler(OBJ_70)
	environment:AddActorObject(OBJ_70H)


	OBJ_71 = ActorObjectInfo(71)
	OBJ_71:SetRenderType(1)
	OBJ_71.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_44.osgb"
	OBJ_71.DisplayDesc.UseLight = false
	OBJ_71:SetModelState(1)
	OBJ_71.PhysicDesc.Pos.X = 521.5
	OBJ_71.PhysicDesc.Pos.Y = 0
	OBJ_71.PhysicDesc.Pos.Z = 565.5
	OBJ_71.PhysicDesc.Pos.Rotation = 90
	OBJ_71:SetPhysicalActorType(1)
	OBJ_71:SetPhysicalShape(5)
	OBJ_71.PhysicDesc.Collidable = true
	OBJ_71.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_44.phy"
	OBJ_71H = ActorHandler(OBJ_71)
	environment:AddActorObject(OBJ_71H)


	OBJ_72 = ActorObjectInfo(72)
	OBJ_72:SetRenderType(1)
	OBJ_72.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_44.osgb"
	OBJ_72.DisplayDesc.UseLight = false
	OBJ_72:SetModelState(1)
	OBJ_72.PhysicDesc.Pos.X = 519.5
	OBJ_72.PhysicDesc.Pos.Y = 0
	OBJ_72.PhysicDesc.Pos.Z = 568.5
	OBJ_72.PhysicDesc.Pos.Rotation = 90
	OBJ_72:SetPhysicalActorType(1)
	OBJ_72:SetPhysicalShape(5)
	OBJ_72.PhysicDesc.Collidable = true
	OBJ_72.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_44.phy"
	OBJ_72H = ActorHandler(OBJ_72)
	environment:AddActorObject(OBJ_72H)


	OBJ_73 = ActorObjectInfo(73)
	OBJ_73:SetRenderType(1)
	OBJ_73.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_44.osgb"
	OBJ_73.DisplayDesc.UseLight = false
	OBJ_73:SetModelState(1)
	OBJ_73.PhysicDesc.Pos.X = 527.5
	OBJ_73.PhysicDesc.Pos.Y = 0
	OBJ_73.PhysicDesc.Pos.Z = 563.5
	OBJ_73.PhysicDesc.Pos.Rotation = 180
	OBJ_73:SetPhysicalActorType(1)
	OBJ_73:SetPhysicalShape(5)
	OBJ_73.PhysicDesc.Collidable = true
	OBJ_73.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_44.phy"
	OBJ_73H = ActorHandler(OBJ_73)
	environment:AddActorObject(OBJ_73H)


	OBJ_74 = ActorObjectInfo(74)
	OBJ_74:SetRenderType(1)
	OBJ_74.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_42.osgb"
	OBJ_74.DisplayDesc.UseLight = false
	OBJ_74:SetModelState(1)
	OBJ_74.PhysicDesc.Pos.X = 533.5
	OBJ_74.PhysicDesc.Pos.Y = 0
	OBJ_74.PhysicDesc.Pos.Z = 580.5
	OBJ_74.PhysicDesc.Pos.Rotation = 180
	OBJ_74:SetPhysicalActorType(1)
	OBJ_74:SetPhysicalShape(5)
	OBJ_74.PhysicDesc.Collidable = true
	OBJ_74.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_42.phy"
	OBJ_74H = ActorHandler(OBJ_74)
	environment:AddActorObject(OBJ_74H)


	OBJ_75 = ActorObjectInfo(75)
	OBJ_75:SetRenderType(1)
	OBJ_75.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_42.osgb"
	OBJ_75.DisplayDesc.UseLight = false
	OBJ_75:SetModelState(1)
	OBJ_75.PhysicDesc.Pos.X = 535.5
	OBJ_75.PhysicDesc.Pos.Y = 0
	OBJ_75.PhysicDesc.Pos.Z = 577.5
	OBJ_75.PhysicDesc.Pos.Rotation = 180
	OBJ_75:SetPhysicalActorType(1)
	OBJ_75:SetPhysicalShape(5)
	OBJ_75.PhysicDesc.Collidable = true
	OBJ_75.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_42.phy"
	OBJ_75H = ActorHandler(OBJ_75)
	environment:AddActorObject(OBJ_75H)


	OBJ_76 = ActorObjectInfo(76)
	OBJ_76:SetRenderType(1)
	OBJ_76.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_44.osgb"
	OBJ_76.DisplayDesc.UseLight = false
	OBJ_76:SetModelState(1)
	OBJ_76.PhysicDesc.Pos.X = 529.5
	OBJ_76.PhysicDesc.Pos.Y = 0
	OBJ_76.PhysicDesc.Pos.Z = 578.5
	OBJ_76.PhysicDesc.Pos.Rotation = 90
	OBJ_76:SetPhysicalActorType(1)
	OBJ_76:SetPhysicalShape(5)
	OBJ_76.PhysicDesc.Collidable = true
	OBJ_76.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_44.phy"
	OBJ_76H = ActorHandler(OBJ_76)
	environment:AddActorObject(OBJ_76H)


	OBJ_77 = ActorObjectInfo(77)
	OBJ_77:SetRenderType(1)
	OBJ_77.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_44.osgb"
	OBJ_77.DisplayDesc.UseLight = false
	OBJ_77:SetModelState(1)
	OBJ_77.PhysicDesc.Pos.X = 535.5
	OBJ_77.PhysicDesc.Pos.Y = 0
	OBJ_77.PhysicDesc.Pos.Z = 579.5
	OBJ_77.PhysicDesc.Pos.Rotation = 90
	OBJ_77:SetPhysicalActorType(1)
	OBJ_77:SetPhysicalShape(5)
	OBJ_77.PhysicDesc.Collidable = true
	OBJ_77.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_44.phy"
	OBJ_77H = ActorHandler(OBJ_77)
	environment:AddActorObject(OBJ_77H)


	OBJ_78 = ActorObjectInfo(78)
	OBJ_78:SetRenderType(1)
	OBJ_78.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_43.osgb"
	OBJ_78.DisplayDesc.UseLight = false
	OBJ_78:SetModelState(1)
	OBJ_78.PhysicDesc.Pos.X = 532.5
	OBJ_78.PhysicDesc.Pos.Y = 0
	OBJ_78.PhysicDesc.Pos.Z = 577.5
	OBJ_78.PhysicDesc.Pos.Rotation = 90
	OBJ_78:SetPhysicalActorType(1)
	OBJ_78:SetPhysicalShape(5)
	OBJ_78.PhysicDesc.Collidable = true
	OBJ_78.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_43.phy"
	OBJ_78H = ActorHandler(OBJ_78)
	environment:AddActorObject(OBJ_78H)


	OBJ_79 = ActorObjectInfo(79)
	OBJ_79:SetRenderType(1)
	OBJ_79.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_43.osgb"
	OBJ_79.DisplayDesc.UseLight = false
	OBJ_79:SetModelState(1)
	OBJ_79.PhysicDesc.Pos.X = 534.5
	OBJ_79.PhysicDesc.Pos.Y = 0
	OBJ_79.PhysicDesc.Pos.Z = 582.5
	OBJ_79.PhysicDesc.Pos.Rotation = 360
	OBJ_79:SetPhysicalActorType(1)
	OBJ_79:SetPhysicalShape(5)
	OBJ_79.PhysicDesc.Collidable = true
	OBJ_79.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_43.phy"
	OBJ_79H = ActorHandler(OBJ_79)
	environment:AddActorObject(OBJ_79H)


	OBJ_80 = ActorObjectInfo(80)
	OBJ_80:SetRenderType(1)
	OBJ_80.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_45.osgb"
	OBJ_80.DisplayDesc.UseLight = false
	OBJ_80:SetModelState(1)
	OBJ_80.PhysicDesc.Pos.X = 511
	OBJ_80.PhysicDesc.Pos.Y = 0
	OBJ_80.PhysicDesc.Pos.Z = 580
	OBJ_80.PhysicDesc.Pos.Rotation = 90
	OBJ_80:SetPhysicalActorType(1)
	OBJ_80:SetPhysicalShape(5)
	OBJ_80.PhysicDesc.Collidable = true
	OBJ_80.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_45.phy"
	OBJ_80H = ActorHandler(OBJ_80)
	environment:AddActorObject(OBJ_80H)


	OBJ_81 = ActorObjectInfo(81)
	OBJ_81:SetRenderType(1)
	OBJ_81.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_46.osgb"
	OBJ_81.DisplayDesc.UseLight = false
	OBJ_81:SetModelState(1)
	OBJ_81.PhysicDesc.Pos.X = 511
	OBJ_81.PhysicDesc.Pos.Y = 0
	OBJ_81.PhysicDesc.Pos.Z = 580
	OBJ_81.PhysicDesc.Pos.Rotation = 90
	OBJ_81:SetPhysicalActorType(1)
	OBJ_81:SetPhysicalShape(5)
	OBJ_81.PhysicDesc.Collidable = true
	OBJ_81.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_46.phy"
	OBJ_81H = ActorHandler(OBJ_81)
	environment:AddActorObject(OBJ_81H)


	OBJ_82 = ActorObjectInfo(82)
	OBJ_82:SetRenderType(1)
	OBJ_82.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_47.osgb"
	OBJ_82.DisplayDesc.UseLight = false
	OBJ_82:SetModelState(1)
	OBJ_82.PhysicDesc.Pos.X = 511
	OBJ_82.PhysicDesc.Pos.Y = 0
	OBJ_82.PhysicDesc.Pos.Z = 580
	OBJ_82.PhysicDesc.Pos.Rotation = 90
	OBJ_82:SetPhysicalActorType(1)
	OBJ_82:SetPhysicalShape(5)
	OBJ_82.PhysicDesc.Collidable = true
	OBJ_82.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_47.phy"
	OBJ_82H = ActorHandler(OBJ_82)
	environment:AddActorObject(OBJ_82H)


	OBJ_83 = ActorObjectInfo(83)
	OBJ_83:SetRenderType(1)
	OBJ_83.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_48.osgb"
	OBJ_83.DisplayDesc.UseLight = false
	OBJ_83:SetModelState(1)
	OBJ_83.PhysicDesc.Pos.X = 511
	OBJ_83.PhysicDesc.Pos.Y = 0
	OBJ_83.PhysicDesc.Pos.Z = 580
	OBJ_83.PhysicDesc.Pos.Rotation = 90
	OBJ_83:SetPhysicalActorType(1)
	OBJ_83:SetPhysicalShape(5)
	OBJ_83.PhysicDesc.Collidable = true
	OBJ_83.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_48.phy"
	OBJ_83H = ActorHandler(OBJ_83)
	environment:AddActorObject(OBJ_83H)


	OBJ_84 = ActorObjectInfo(84)
	OBJ_84:SetRenderType(1)
	OBJ_84.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_49.osgb"
	OBJ_84.DisplayDesc.UseLight = false
	OBJ_84:SetModelState(1)
	OBJ_84.PhysicDesc.Pos.X = 511
	OBJ_84.PhysicDesc.Pos.Y = 0
	OBJ_84.PhysicDesc.Pos.Z = 580
	OBJ_84.PhysicDesc.Pos.Rotation = 90
	OBJ_84:SetPhysicalActorType(1)
	OBJ_84:SetPhysicalShape(5)
	OBJ_84.PhysicDesc.Collidable = true
	OBJ_84.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_49.phy"
	OBJ_84H = ActorHandler(OBJ_84)
	environment:AddActorObject(OBJ_84H)


	OBJ_85 = ActorObjectInfo(85)
	OBJ_85:SetRenderType(1)
	OBJ_85.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_50.osgb"
	OBJ_85.DisplayDesc.UseLight = false
	OBJ_85:SetModelState(1)
	OBJ_85.PhysicDesc.Pos.X = 511
	OBJ_85.PhysicDesc.Pos.Y = 0
	OBJ_85.PhysicDesc.Pos.Z = 580
	OBJ_85.PhysicDesc.Pos.Rotation = 90
	OBJ_85:SetPhysicalActorType(1)
	OBJ_85:SetPhysicalShape(5)
	OBJ_85.PhysicDesc.Collidable = true
	OBJ_85.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_50.phy"
	OBJ_85H = ActorHandler(OBJ_85)
	environment:AddActorObject(OBJ_85H)


	OBJ_86 = ActorObjectInfo(86)
	OBJ_86:SetRenderType(1)
	OBJ_86.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_51.osgb"
	OBJ_86.DisplayDesc.UseLight = false
	OBJ_86:SetModelState(1)
	OBJ_86.PhysicDesc.Pos.X = 511
	OBJ_86.PhysicDesc.Pos.Y = 0
	OBJ_86.PhysicDesc.Pos.Z = 580
	OBJ_86.PhysicDesc.Pos.Rotation = 90
	OBJ_86:SetPhysicalActorType(1)
	OBJ_86:SetPhysicalShape(5)
	OBJ_86.PhysicDesc.Collidable = true
	OBJ_86.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_51.phy"
	OBJ_86H = ActorHandler(OBJ_86)
	environment:AddActorObject(OBJ_86H)


	OBJ_87 = ActorObjectInfo(87)
	OBJ_87:SetRenderType(1)
	OBJ_87.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_52.osgb"
	OBJ_87.DisplayDesc.UseLight = false
	OBJ_87:SetModelState(1)
	OBJ_87.PhysicDesc.Pos.X = 511
	OBJ_87.PhysicDesc.Pos.Y = 0
	OBJ_87.PhysicDesc.Pos.Z = 580
	OBJ_87.PhysicDesc.Pos.Rotation = 90
	OBJ_87:SetPhysicalActorType(1)
	OBJ_87:SetPhysicalShape(5)
	OBJ_87.PhysicDesc.Collidable = true
	OBJ_87.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_52.phy"
	OBJ_87H = ActorHandler(OBJ_87)
	environment:AddActorObject(OBJ_87H)


	OBJ_88 = ActorObjectInfo(88)
	OBJ_88:SetRenderType(1)
	OBJ_88.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_53.osgb"
	OBJ_88.DisplayDesc.UseLight = false
	OBJ_88:SetModelState(1)
	OBJ_88.PhysicDesc.Pos.X = 511
	OBJ_88.PhysicDesc.Pos.Y = 0
	OBJ_88.PhysicDesc.Pos.Z = 580
	OBJ_88.PhysicDesc.Pos.Rotation = 90
	OBJ_88:SetPhysicalActorType(1)
	OBJ_88:SetPhysicalShape(5)
	OBJ_88.PhysicDesc.Collidable = true
	OBJ_88.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_53.phy"
	OBJ_88H = ActorHandler(OBJ_88)
	environment:AddActorObject(OBJ_88H)


	OBJ_89 = ActorObjectInfo(89)
	OBJ_89:SetRenderType(1)
	OBJ_89.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_54.osgb"
	OBJ_89.DisplayDesc.UseLight = false
	OBJ_89:SetModelState(1)
	OBJ_89.PhysicDesc.Pos.X = 511
	OBJ_89.PhysicDesc.Pos.Y = 0
	OBJ_89.PhysicDesc.Pos.Z = 580
	OBJ_89.PhysicDesc.Pos.Rotation = 90
	OBJ_89:SetPhysicalActorType(1)
	OBJ_89:SetPhysicalShape(5)
	OBJ_89.PhysicDesc.Collidable = true
	OBJ_89.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_54.phy"
	OBJ_89H = ActorHandler(OBJ_89)
	environment:AddActorObject(OBJ_89H)


	OBJ_90 = ActorObjectInfo(90)
	OBJ_90:SetRenderType(1)
	OBJ_90.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_55.osgb"
	OBJ_90.DisplayDesc.UseLight = false
	OBJ_90:SetModelState(1)
	OBJ_90.PhysicDesc.Pos.X = 511
	OBJ_90.PhysicDesc.Pos.Y = 0
	OBJ_90.PhysicDesc.Pos.Z = 580
	OBJ_90.PhysicDesc.Pos.Rotation = 90
	OBJ_90:SetPhysicalActorType(1)
	OBJ_90:SetPhysicalShape(5)
	OBJ_90.PhysicDesc.Collidable = true
	OBJ_90.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_55.phy"
	OBJ_90H = ActorHandler(OBJ_90)
	environment:AddActorObject(OBJ_90H)


	OBJ_91 = ActorObjectInfo(91)
	OBJ_91:SetRenderType(1)
	OBJ_91.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_56.osgb"
	OBJ_91.DisplayDesc.UseLight = false
	OBJ_91:SetModelState(1)
	OBJ_91.PhysicDesc.Pos.X = 511
	OBJ_91.PhysicDesc.Pos.Y = 0
	OBJ_91.PhysicDesc.Pos.Z = 580
	OBJ_91.PhysicDesc.Pos.Rotation = 90
	OBJ_91:SetPhysicalActorType(1)
	OBJ_91:SetPhysicalShape(5)
	OBJ_91.PhysicDesc.Collidable = true
	OBJ_91.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_56.phy"
	OBJ_91H = ActorHandler(OBJ_91)
	environment:AddActorObject(OBJ_91H)


	OBJ_92 = ActorObjectInfo(92)
	OBJ_92:SetRenderType(1)
	OBJ_92.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_57.osgb"
	OBJ_92.DisplayDesc.UseLight = false
	OBJ_92:SetModelState(1)
	OBJ_92.PhysicDesc.Pos.X = 511
	OBJ_92.PhysicDesc.Pos.Y = 0
	OBJ_92.PhysicDesc.Pos.Z = 580
	OBJ_92.PhysicDesc.Pos.Rotation = 90
	OBJ_92:SetPhysicalActorType(1)
	OBJ_92:SetPhysicalShape(5)
	OBJ_92.PhysicDesc.Collidable = true
	OBJ_92.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_57.phy"
	OBJ_92H = ActorHandler(OBJ_92)
	environment:AddActorObject(OBJ_92H)


	OBJ_93 = ActorObjectInfo(93)
	OBJ_93:SetRenderType(1)
	OBJ_93.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_58.osgb"
	OBJ_93.DisplayDesc.UseLight = false
	OBJ_93:SetModelState(1)
	OBJ_93.PhysicDesc.Pos.X = 511
	OBJ_93.PhysicDesc.Pos.Y = 0
	OBJ_93.PhysicDesc.Pos.Z = 580
	OBJ_93.PhysicDesc.Pos.Rotation = 90
	OBJ_93:SetPhysicalActorType(1)
	OBJ_93:SetPhysicalShape(5)
	OBJ_93.PhysicDesc.Collidable = true
	OBJ_93.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_58.phy"
	OBJ_93H = ActorHandler(OBJ_93)
	environment:AddActorObject(OBJ_93H)


	OBJ_94 = ActorObjectInfo(94)
	OBJ_94:SetRenderType(1)
	OBJ_94.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_59.osgb"
	OBJ_94.DisplayDesc.UseLight = false
	OBJ_94:SetModelState(1)
	OBJ_94.PhysicDesc.Pos.X = 511
	OBJ_94.PhysicDesc.Pos.Y = 0
	OBJ_94.PhysicDesc.Pos.Z = 580
	OBJ_94.PhysicDesc.Pos.Rotation = 90
	OBJ_94:SetPhysicalActorType(1)
	OBJ_94:SetPhysicalShape(5)
	OBJ_94.PhysicDesc.Collidable = true
	OBJ_94.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_59.phy"
	OBJ_94H = ActorHandler(OBJ_94)
	environment:AddActorObject(OBJ_94H)


	OBJ_95 = ActorObjectInfo(95)
	OBJ_95:SetRenderType(1)
	OBJ_95.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_60.osgb"
	OBJ_95.DisplayDesc.UseLight = false
	OBJ_95:SetModelState(1)
	OBJ_95.PhysicDesc.Pos.X = 511
	OBJ_95.PhysicDesc.Pos.Y = 0
	OBJ_95.PhysicDesc.Pos.Z = 580
	OBJ_95.PhysicDesc.Pos.Rotation = 90
	OBJ_95:SetPhysicalActorType(1)
	OBJ_95:SetPhysicalShape(5)
	OBJ_95.PhysicDesc.Collidable = true
	OBJ_95.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_60.phy"
	OBJ_95H = ActorHandler(OBJ_95)
	environment:AddActorObject(OBJ_95H)


	OBJ_96 = ActorObjectInfo(96)
	OBJ_96:SetRenderType(1)
	OBJ_96.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_61.osgb"
	OBJ_96.DisplayDesc.UseLight = false
	OBJ_96:SetModelState(1)
	OBJ_96.PhysicDesc.Pos.X = 511
	OBJ_96.PhysicDesc.Pos.Y = 0
	OBJ_96.PhysicDesc.Pos.Z = 580
	OBJ_96.PhysicDesc.Pos.Rotation = 90
	OBJ_96:SetPhysicalActorType(1)
	OBJ_96:SetPhysicalShape(5)
	OBJ_96.PhysicDesc.Collidable = true
	OBJ_96.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_61.phy"
	OBJ_96H = ActorHandler(OBJ_96)
	environment:AddActorObject(OBJ_96H)


	OBJ_97 = ActorObjectInfo(97)
	OBJ_97:SetRenderType(1)
	OBJ_97.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_62.osgb"
	OBJ_97.DisplayDesc.UseLight = false
	OBJ_97:SetModelState(1)
	OBJ_97.PhysicDesc.Pos.X = 511
	OBJ_97.PhysicDesc.Pos.Y = 0
	OBJ_97.PhysicDesc.Pos.Z = 580
	OBJ_97.PhysicDesc.Pos.Rotation = 90
	OBJ_97:SetPhysicalActorType(1)
	OBJ_97:SetPhysicalShape(5)
	OBJ_97.PhysicDesc.Collidable = true
	OBJ_97.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_62.phy"
	OBJ_97H = ActorHandler(OBJ_97)
	environment:AddActorObject(OBJ_97H)


	OBJ_98 = ActorObjectInfo(98)
	OBJ_98:SetRenderType(1)
	OBJ_98.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_63.osgb"
	OBJ_98.DisplayDesc.UseLight = false
	OBJ_98:SetModelState(1)
	OBJ_98.PhysicDesc.Pos.X = 511
	OBJ_98.PhysicDesc.Pos.Y = 0
	OBJ_98.PhysicDesc.Pos.Z = 580
	OBJ_98.PhysicDesc.Pos.Rotation = 90
	OBJ_98:SetPhysicalActorType(1)
	OBJ_98:SetPhysicalShape(5)
	OBJ_98.PhysicDesc.Collidable = true
	OBJ_98.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_63.phy"
	OBJ_98H = ActorHandler(OBJ_98)
	environment:AddActorObject(OBJ_98H)


	OBJ_99 = ActorObjectInfo(99)
	OBJ_99:SetRenderType(1)
	OBJ_99.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_64.osgb"
	OBJ_99.DisplayDesc.UseLight = false
	OBJ_99:SetModelState(1)
	OBJ_99.PhysicDesc.Pos.X = 511
	OBJ_99.PhysicDesc.Pos.Y = 0
	OBJ_99.PhysicDesc.Pos.Z = 580
	OBJ_99.PhysicDesc.Pos.Rotation = 90
	OBJ_99:SetPhysicalActorType(1)
	OBJ_99:SetPhysicalShape(5)
	OBJ_99.PhysicDesc.Collidable = true
	OBJ_99.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_64.phy"
	OBJ_99H = ActorHandler(OBJ_99)
	environment:AddActorObject(OBJ_99H)


	OBJ_100 = ActorObjectInfo(100)
	OBJ_100:SetRenderType(1)
	OBJ_100.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_65.osgb"
	OBJ_100.DisplayDesc.UseLight = false
	OBJ_100:SetModelState(1)
	OBJ_100.PhysicDesc.Pos.X = 511
	OBJ_100.PhysicDesc.Pos.Y = 0
	OBJ_100.PhysicDesc.Pos.Z = 580
	OBJ_100.PhysicDesc.Pos.Rotation = 90
	OBJ_100:SetPhysicalActorType(1)
	OBJ_100:SetPhysicalShape(5)
	OBJ_100.PhysicDesc.Collidable = true
	OBJ_100.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_65.phy"
	OBJ_100H = ActorHandler(OBJ_100)
	environment:AddActorObject(OBJ_100H)


	OBJ_101 = ActorObjectInfo(101)
	OBJ_101:SetRenderType(1)
	OBJ_101.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_66.osgb"
	OBJ_101.DisplayDesc.UseLight = false
	OBJ_101:SetModelState(1)
	OBJ_101.PhysicDesc.Pos.X = 511
	OBJ_101.PhysicDesc.Pos.Y = 0
	OBJ_101.PhysicDesc.Pos.Z = 580
	OBJ_101.PhysicDesc.Pos.Rotation = 90
	OBJ_101:SetPhysicalActorType(1)
	OBJ_101:SetPhysicalShape(5)
	OBJ_101.PhysicDesc.Collidable = true
	OBJ_101.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_66.phy"
	OBJ_101H = ActorHandler(OBJ_101)
	environment:AddActorObject(OBJ_101H)


	OBJ_102 = ActorObjectInfo(102)
	OBJ_102:SetRenderType(1)
	OBJ_102.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_67.osgb"
	OBJ_102.DisplayDesc.UseLight = false
	OBJ_102:SetModelState(1)
	OBJ_102.PhysicDesc.Pos.X = 511
	OBJ_102.PhysicDesc.Pos.Y = 0
	OBJ_102.PhysicDesc.Pos.Z = 580
	OBJ_102.PhysicDesc.Pos.Rotation = 90
	OBJ_102:SetPhysicalActorType(1)
	OBJ_102:SetPhysicalShape(5)
	OBJ_102.PhysicDesc.Collidable = true
	OBJ_102.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_67.phy"
	OBJ_102H = ActorHandler(OBJ_102)
	environment:AddActorObject(OBJ_102H)


	OBJ_103 = ActorObjectInfo(103)
	OBJ_103:SetRenderType(1)
	OBJ_103.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_68.osgb"
	OBJ_103.DisplayDesc.UseLight = false
	OBJ_103:SetModelState(1)
	OBJ_103.PhysicDesc.Pos.X = 511
	OBJ_103.PhysicDesc.Pos.Y = 0
	OBJ_103.PhysicDesc.Pos.Z = 580
	OBJ_103.PhysicDesc.Pos.Rotation = 90
	OBJ_103:SetPhysicalActorType(1)
	OBJ_103:SetPhysicalShape(5)
	OBJ_103.PhysicDesc.Collidable = true
	OBJ_103.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_68.phy"
	OBJ_103H = ActorHandler(OBJ_103)
	environment:AddActorObject(OBJ_103H)


	OBJ_104 = ActorObjectInfo(104)
	OBJ_104:SetRenderType(1)
	OBJ_104.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_69.osgb"
	OBJ_104.DisplayDesc.UseLight = false
	OBJ_104:SetModelState(1)
	OBJ_104.PhysicDesc.Pos.X = 511
	OBJ_104.PhysicDesc.Pos.Y = 0
	OBJ_104.PhysicDesc.Pos.Z = 580
	OBJ_104.PhysicDesc.Pos.Rotation = 90
	OBJ_104:SetPhysicalActorType(1)
	OBJ_104:SetPhysicalShape(5)
	OBJ_104.PhysicDesc.Collidable = true
	OBJ_104.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_69.phy"
	OBJ_104H = ActorHandler(OBJ_104)
	environment:AddActorObject(OBJ_104H)


	OBJ_105 = ActorObjectInfo(105)
	OBJ_105:SetRenderType(1)
	OBJ_105.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_70.osgb"
	OBJ_105.DisplayDesc.UseLight = false
	OBJ_105:SetModelState(1)
	OBJ_105.PhysicDesc.Pos.X = 511
	OBJ_105.PhysicDesc.Pos.Y = 0
	OBJ_105.PhysicDesc.Pos.Z = 580
	OBJ_105.PhysicDesc.Pos.Rotation = 90
	OBJ_105:SetPhysicalActorType(1)
	OBJ_105:SetPhysicalShape(5)
	OBJ_105.PhysicDesc.Collidable = true
	OBJ_105.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_70.phy"
	OBJ_105H = ActorHandler(OBJ_105)
	environment:AddActorObject(OBJ_105H)


	OBJ_106 = ActorObjectInfo(106)
	OBJ_106:SetRenderType(1)
	OBJ_106.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_71.osgb"
	OBJ_106.DisplayDesc.UseLight = false
	OBJ_106:SetModelState(1)
	OBJ_106.PhysicDesc.Pos.X = 511
	OBJ_106.PhysicDesc.Pos.Y = 0
	OBJ_106.PhysicDesc.Pos.Z = 580
	OBJ_106.PhysicDesc.Pos.Rotation = 90
	OBJ_106:SetPhysicalActorType(1)
	OBJ_106:SetPhysicalShape(5)
	OBJ_106.PhysicDesc.Collidable = true
	OBJ_106.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_71.phy"
	OBJ_106H = ActorHandler(OBJ_106)
	environment:AddActorObject(OBJ_106H)


	OBJ_107 = ActorObjectInfo(107)
	OBJ_107:SetRenderType(1)
	OBJ_107.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_72.osgb"
	OBJ_107.DisplayDesc.UseLight = false
	OBJ_107:SetModelState(1)
	OBJ_107.PhysicDesc.Pos.X = 511
	OBJ_107.PhysicDesc.Pos.Y = 0
	OBJ_107.PhysicDesc.Pos.Z = 580
	OBJ_107.PhysicDesc.Pos.Rotation = 90
	OBJ_107:SetPhysicalActorType(1)
	OBJ_107:SetPhysicalShape(5)
	OBJ_107.PhysicDesc.Collidable = true
	OBJ_107.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_72.phy"
	OBJ_107H = ActorHandler(OBJ_107)
	environment:AddActorObject(OBJ_107H)


	OBJ_108 = ActorObjectInfo(108)
	OBJ_108:SetRenderType(1)
	OBJ_108.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_73.osgb"
	OBJ_108.DisplayDesc.UseLight = false
	OBJ_108:SetModelState(1)
	OBJ_108.PhysicDesc.Pos.X = 511
	OBJ_108.PhysicDesc.Pos.Y = 0
	OBJ_108.PhysicDesc.Pos.Z = 580
	OBJ_108.PhysicDesc.Pos.Rotation = 90
	OBJ_108:SetPhysicalActorType(1)
	OBJ_108:SetPhysicalShape(5)
	OBJ_108.PhysicDesc.Collidable = true
	OBJ_108.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_73.phy"
	OBJ_108H = ActorHandler(OBJ_108)
	environment:AddActorObject(OBJ_108H)


	OBJ_109 = ActorObjectInfo(109)
	OBJ_109:SetRenderType(1)
	OBJ_109.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_74.osgb"
	OBJ_109.DisplayDesc.UseLight = false
	OBJ_109:SetModelState(1)
	OBJ_109.PhysicDesc.Pos.X = 511
	OBJ_109.PhysicDesc.Pos.Y = 0
	OBJ_109.PhysicDesc.Pos.Z = 580
	OBJ_109.PhysicDesc.Pos.Rotation = 90
	OBJ_109:SetPhysicalActorType(1)
	OBJ_109:SetPhysicalShape(5)
	OBJ_109.PhysicDesc.Collidable = true
	OBJ_109.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_74.phy"
	OBJ_109H = ActorHandler(OBJ_109)
	environment:AddActorObject(OBJ_109H)


	OBJ_110 = ActorObjectInfo(110)
	OBJ_110:SetRenderType(1)
	OBJ_110.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_75.osgb"
	OBJ_110.DisplayDesc.UseLight = false
	OBJ_110:SetModelState(1)
	OBJ_110.PhysicDesc.Pos.X = 511
	OBJ_110.PhysicDesc.Pos.Y = 0
	OBJ_110.PhysicDesc.Pos.Z = 580
	OBJ_110.PhysicDesc.Pos.Rotation = 90
	OBJ_110:SetPhysicalActorType(1)
	OBJ_110:SetPhysicalShape(5)
	OBJ_110.PhysicDesc.Collidable = true
	OBJ_110.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_75.phy"
	OBJ_110H = ActorHandler(OBJ_110)
	environment:AddActorObject(OBJ_110H)


	OBJ_111 = ActorObjectInfo(111)
	OBJ_111:SetRenderType(1)
	OBJ_111.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_76.osgb"
	OBJ_111.DisplayDesc.UseLight = false
	OBJ_111:SetModelState(1)
	OBJ_111.PhysicDesc.Pos.X = 522.5
	OBJ_111.PhysicDesc.Pos.Y = 0.683594
	OBJ_111.PhysicDesc.Pos.Z = 578.875
	OBJ_111.PhysicDesc.Pos.Rotation = 270
	OBJ_111:SetPhysicalActorType(1)
	OBJ_111:SetPhysicalShape(5)
	OBJ_111.PhysicDesc.Collidable = true
	OBJ_111.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_76.phy"
	OBJ_111H = ActorHandler(OBJ_111)
	environment:AddActorObject(OBJ_111H)


	OBJ_112 = ActorObjectInfo(112)
	OBJ_112:SetRenderType(1)
	OBJ_112.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_77.osgb"
	OBJ_112.DisplayDesc.UseLight = false
	OBJ_112:SetModelState(1)
	OBJ_112.PhysicDesc.Pos.X = 522.5
	OBJ_112.PhysicDesc.Pos.Y = 0.683594
	OBJ_112.PhysicDesc.Pos.Z = 578.875
	OBJ_112.PhysicDesc.Pos.Rotation = 270
	OBJ_112:SetPhysicalActorType(1)
	OBJ_112:SetPhysicalShape(5)
	OBJ_112.PhysicDesc.Collidable = true
	OBJ_112.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_77.phy"
	OBJ_112H = ActorHandler(OBJ_112)
	environment:AddActorObject(OBJ_112H)


	OBJ_113 = ActorObjectInfo(113)
	OBJ_113:SetRenderType(1)
	OBJ_113.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_16.osgb"
	OBJ_113.DisplayDesc.UseLight = false
	OBJ_113:SetModelState(1)
	OBJ_113.PhysicDesc.Pos.X = 506.625
	OBJ_113.PhysicDesc.Pos.Y = 11.1797
	OBJ_113.PhysicDesc.Pos.Z = 558.5
	OBJ_113.PhysicDesc.Pos.Rotation = 360
	OBJ_113:SetPhysicalActorType(1)
	OBJ_113:SetPhysicalShape(5)
	OBJ_113.PhysicDesc.Collidable = true
	OBJ_113.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_16.phy"
	OBJ_113H = ActorHandler(OBJ_113)
	environment:AddActorObject(OBJ_113H)


	OBJ_114 = ActorObjectInfo(114)
	OBJ_114:SetRenderType(1)
	OBJ_114.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_34.osgb"
	OBJ_114.DisplayDesc.UseLight = false
	OBJ_114:SetModelState(1)
	OBJ_114.PhysicDesc.Pos.X = 514.5
	OBJ_114.PhysicDesc.Pos.Y = 12.207
	OBJ_114.PhysicDesc.Pos.Z = 553.5
	OBJ_114.PhysicDesc.Pos.Rotation = 360
	OBJ_114:SetPhysicalActorType(1)
	OBJ_114:SetPhysicalShape(5)
	OBJ_114.PhysicDesc.Collidable = true
	OBJ_114.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_34.phy"
	OBJ_114H = ActorHandler(OBJ_114)
	environment:AddActorObject(OBJ_114H)


	OBJ_115 = ActorObjectInfo(115)
	OBJ_115:SetRenderType(1)
	OBJ_115.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_78.osgb"
	OBJ_115.DisplayDesc.UseLight = false
	OBJ_115:SetModelState(1)
	OBJ_115.PhysicDesc.Pos.X = 525.625
	OBJ_115.PhysicDesc.Pos.Y = 9.76172
	OBJ_115.PhysicDesc.Pos.Z = 590.375
	OBJ_115.PhysicDesc.Pos.Rotation = 270
	OBJ_115:SetPhysicalActorType(1)
	OBJ_115:SetPhysicalShape(5)
	OBJ_115.PhysicDesc.Collidable = true
	OBJ_115.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_78.phy"
	OBJ_115H = ActorHandler(OBJ_115)
	environment:AddActorObject(OBJ_115H)


	OBJ_116 = ActorObjectInfo(116)
	OBJ_116:SetRenderType(1)
	OBJ_116.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_79.osgb"
	OBJ_116.DisplayDesc.UseLight = false
	OBJ_116:SetModelState(1)
	OBJ_116.PhysicDesc.Pos.X = 525.625
	OBJ_116.PhysicDesc.Pos.Y = 9.76563
	OBJ_116.PhysicDesc.Pos.Z = 590.375
	OBJ_116.PhysicDesc.Pos.Rotation = 270
	OBJ_116:SetPhysicalActorType(1)
	OBJ_116:SetPhysicalShape(5)
	OBJ_116.PhysicDesc.Collidable = true
	OBJ_116.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_79.phy"
	OBJ_116H = ActorHandler(OBJ_116)
	environment:AddActorObject(OBJ_116H)


	OBJ_117 = ActorObjectInfo(117)
	OBJ_117:SetRenderType(1)
	OBJ_117.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_80.osgb"
	OBJ_117.DisplayDesc.UseLight = false
	OBJ_117:SetModelState(1)
	OBJ_117.PhysicDesc.Pos.X = 525.875
	OBJ_117.PhysicDesc.Pos.Y = 9.72266
	OBJ_117.PhysicDesc.Pos.Z = 590.625
	OBJ_117.PhysicDesc.Pos.Rotation = 270
	OBJ_117:SetPhysicalActorType(1)
	OBJ_117:SetPhysicalShape(5)
	OBJ_117.PhysicDesc.Collidable = true
	OBJ_117.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_80.phy"
	OBJ_117H = ActorHandler(OBJ_117)
	environment:AddActorObject(OBJ_117H)


	OBJ_118 = ActorObjectInfo(118)
	OBJ_118:SetRenderType(1)
	OBJ_118.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_80.osgb"
	OBJ_118.DisplayDesc.UseLight = false
	OBJ_118:SetModelState(1)
	OBJ_118.PhysicDesc.Pos.X = 525.375
	OBJ_118.PhysicDesc.Pos.Y = 9.72266
	OBJ_118.PhysicDesc.Pos.Z = 590.625
	OBJ_118.PhysicDesc.Pos.Rotation = 180
	OBJ_118:SetPhysicalActorType(1)
	OBJ_118:SetPhysicalShape(5)
	OBJ_118.PhysicDesc.Collidable = true
	OBJ_118.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_80.phy"
	OBJ_118H = ActorHandler(OBJ_118)
	environment:AddActorObject(OBJ_118H)


	OBJ_119 = ActorObjectInfo(119)
	OBJ_119:SetRenderType(1)
	OBJ_119.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_80.osgb"
	OBJ_119.DisplayDesc.UseLight = false
	OBJ_119:SetModelState(1)
	OBJ_119.PhysicDesc.Pos.X = 525.375
	OBJ_119.PhysicDesc.Pos.Y = 9.72266
	OBJ_119.PhysicDesc.Pos.Z = 590.125
	OBJ_119.PhysicDesc.Pos.Rotation = 90
	OBJ_119:SetPhysicalActorType(1)
	OBJ_119:SetPhysicalShape(5)
	OBJ_119.PhysicDesc.Collidable = true
	OBJ_119.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_80.phy"
	OBJ_119H = ActorHandler(OBJ_119)
	environment:AddActorObject(OBJ_119H)


	OBJ_120 = ActorObjectInfo(120)
	OBJ_120:SetRenderType(1)
	OBJ_120.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_80.osgb"
	OBJ_120.DisplayDesc.UseLight = false
	OBJ_120:SetModelState(1)
	OBJ_120.PhysicDesc.Pos.X = 526
	OBJ_120.PhysicDesc.Pos.Y = 9.72266
	OBJ_120.PhysicDesc.Pos.Z = 590.125
	OBJ_120.PhysicDesc.Pos.Rotation = 360
	OBJ_120:SetPhysicalActorType(1)
	OBJ_120:SetPhysicalShape(5)
	OBJ_120.PhysicDesc.Collidable = true
	OBJ_120.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_80.phy"
	OBJ_120H = ActorHandler(OBJ_120)
	environment:AddActorObject(OBJ_120H)


	OBJ_121 = ActorObjectInfo(121)
	OBJ_121:SetRenderType(1)
	OBJ_121.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_81.osgb"
	OBJ_121.DisplayDesc.UseLight = false
	OBJ_121:SetModelState(1)
	OBJ_121.PhysicDesc.Pos.X = 525.625
	OBJ_121.PhysicDesc.Pos.Y = 9.72266
	OBJ_121.PhysicDesc.Pos.Z = 590.375
	OBJ_121.PhysicDesc.Pos.Rotation = 270
	OBJ_121:SetPhysicalActorType(1)
	OBJ_121:SetPhysicalShape(5)
	OBJ_121.PhysicDesc.Collidable = true
	OBJ_121.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_81.phy"
	OBJ_121H = ActorHandler(OBJ_121)
	environment:AddActorObject(OBJ_121H)


	OBJ_122 = ActorObjectInfo(122)
	OBJ_122:SetRenderType(1)
	OBJ_122.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_82.osgb"
	OBJ_122.DisplayDesc.UseLight = false
	OBJ_122:SetModelState(1)
	OBJ_122.PhysicDesc.Pos.X = 525.625
	OBJ_122.PhysicDesc.Pos.Y = 9.72266
	OBJ_122.PhysicDesc.Pos.Z = 590.375
	OBJ_122.PhysicDesc.Pos.Rotation = 270
	OBJ_122:SetPhysicalActorType(1)
	OBJ_122:SetPhysicalShape(5)
	OBJ_122.PhysicDesc.Collidable = true
	OBJ_122.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_82.phy"
	OBJ_122H = ActorHandler(OBJ_122)
	environment:AddActorObject(OBJ_122H)


	OBJ_123 = ActorObjectInfo(123)
	OBJ_123:SetRenderType(1)
	OBJ_123.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_82.osgb"
	OBJ_123.DisplayDesc.UseLight = false
	OBJ_123:SetModelState(1)
	OBJ_123.PhysicDesc.Pos.X = 525.625
	OBJ_123.PhysicDesc.Pos.Y = 9.72266
	OBJ_123.PhysicDesc.Pos.Z = 590.375
	OBJ_123.PhysicDesc.Pos.Rotation = 360
	OBJ_123:SetPhysicalActorType(1)
	OBJ_123:SetPhysicalShape(5)
	OBJ_123.PhysicDesc.Collidable = true
	OBJ_123.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_82.phy"
	OBJ_123H = ActorHandler(OBJ_123)
	environment:AddActorObject(OBJ_123H)


	OBJ_124 = ActorObjectInfo(124)
	OBJ_124:SetRenderType(1)
	OBJ_124.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_82.osgb"
	OBJ_124.DisplayDesc.UseLight = false
	OBJ_124:SetModelState(1)
	OBJ_124.PhysicDesc.Pos.X = 525.625
	OBJ_124.PhysicDesc.Pos.Y = 9.72266
	OBJ_124.PhysicDesc.Pos.Z = 590.375
	OBJ_124.PhysicDesc.Pos.Rotation = 90
	OBJ_124:SetPhysicalActorType(1)
	OBJ_124:SetPhysicalShape(5)
	OBJ_124.PhysicDesc.Collidable = true
	OBJ_124.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_82.phy"
	OBJ_124H = ActorHandler(OBJ_124)
	environment:AddActorObject(OBJ_124H)


	OBJ_125 = ActorObjectInfo(125)
	OBJ_125:SetRenderType(1)
	OBJ_125.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_82.osgb"
	OBJ_125.DisplayDesc.UseLight = false
	OBJ_125:SetModelState(1)
	OBJ_125.PhysicDesc.Pos.X = 525.625
	OBJ_125.PhysicDesc.Pos.Y = 9.72266
	OBJ_125.PhysicDesc.Pos.Z = 590.375
	OBJ_125.PhysicDesc.Pos.Rotation = 180
	OBJ_125:SetPhysicalActorType(1)
	OBJ_125:SetPhysicalShape(5)
	OBJ_125.PhysicDesc.Collidable = true
	OBJ_125.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_82.phy"
	OBJ_125H = ActorHandler(OBJ_125)
	environment:AddActorObject(OBJ_125H)


	OBJ_126 = ActorObjectInfo(126)
	OBJ_126:SetRenderType(1)
	OBJ_126.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_83.osgb"
	OBJ_126.DisplayDesc.UseLight = false
	OBJ_126:SetModelState(1)
	OBJ_126.PhysicDesc.Pos.X = 500
	OBJ_126.PhysicDesc.Pos.Y = 11.1328
	OBJ_126.PhysicDesc.Pos.Z = 505
	OBJ_126.PhysicDesc.Pos.Rotation = 90
	OBJ_126:SetPhysicalActorType(1)
	OBJ_126:SetPhysicalShape(5)
	OBJ_126.PhysicDesc.Collidable = true
	OBJ_126.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_83.phy"
	OBJ_126H = ActorHandler(OBJ_126)
	environment:AddActorObject(OBJ_126H)


	OBJ_127 = ActorObjectInfo(127)
	OBJ_127:SetRenderType(1)
	OBJ_127.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_84.osgb"
	OBJ_127.DisplayDesc.UseLight = false
	OBJ_127:SetModelState(1)
	OBJ_127.PhysicDesc.Pos.X = 497.25
	OBJ_127.PhysicDesc.Pos.Y = 10.5469
	OBJ_127.PhysicDesc.Pos.Z = 502
	OBJ_127.PhysicDesc.Pos.Rotation = 180
	OBJ_127:SetPhysicalActorType(1)
	OBJ_127:SetPhysicalShape(5)
	OBJ_127.PhysicDesc.Collidable = true
	OBJ_127.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_84.phy"
	OBJ_127H = ActorHandler(OBJ_127)
	environment:AddActorObject(OBJ_127H)


	OBJ_128 = ActorObjectInfo(128)
	OBJ_128:SetRenderType(1)
	OBJ_128.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_85.osgb"
	OBJ_128.DisplayDesc.UseLight = false
	OBJ_128:SetModelState(1)
	OBJ_128.PhysicDesc.Pos.X = 513
	OBJ_128.PhysicDesc.Pos.Y = 11.1328
	OBJ_128.PhysicDesc.Pos.Z = 506
	OBJ_128.PhysicDesc.Pos.Rotation = 90
	OBJ_128:SetPhysicalActorType(1)
	OBJ_128:SetPhysicalShape(5)
	OBJ_128.PhysicDesc.Collidable = true
	OBJ_128.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_85.phy"
	OBJ_128H = ActorHandler(OBJ_128)
	environment:AddActorObject(OBJ_128H)


	OBJ_129 = ActorObjectInfo(129)
	OBJ_129:SetRenderType(1)
	OBJ_129.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_25.osgb"
	OBJ_129.DisplayDesc.UseLight = false
	OBJ_129:SetModelState(1)
	OBJ_129.PhysicDesc.Pos.X = 485.5
	OBJ_129.PhysicDesc.Pos.Y = 10.8398
	OBJ_129.PhysicDesc.Pos.Z = 493.5
	OBJ_129.PhysicDesc.Pos.Rotation = 270
	OBJ_129:SetPhysicalActorType(1)
	OBJ_129:SetPhysicalShape(5)
	OBJ_129.PhysicDesc.Collidable = true
	OBJ_129.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_25.phy"
	OBJ_129H = ActorHandler(OBJ_129)
	environment:AddActorObject(OBJ_129H)


	OBJ_130 = ActorObjectInfo(130)
	OBJ_130:SetRenderType(1)
	OBJ_130.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_25.osgb"
	OBJ_130.DisplayDesc.UseLight = false
	OBJ_130:SetModelState(1)
	OBJ_130.PhysicDesc.Pos.X = 491.5
	OBJ_130.PhysicDesc.Pos.Y = 11.1328
	OBJ_130.PhysicDesc.Pos.Z = 498.5
	OBJ_130.PhysicDesc.Pos.Rotation = 270
	OBJ_130:SetPhysicalActorType(1)
	OBJ_130:SetPhysicalShape(5)
	OBJ_130.PhysicDesc.Collidable = true
	OBJ_130.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_25.phy"
	OBJ_130H = ActorHandler(OBJ_130)
	environment:AddActorObject(OBJ_130H)


	OBJ_131 = ActorObjectInfo(131)
	OBJ_131:SetRenderType(1)
	OBJ_131.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_25.osgb"
	OBJ_131.DisplayDesc.UseLight = false
	OBJ_131:SetModelState(1)
	OBJ_131.PhysicDesc.Pos.X = 491.5
	OBJ_131.PhysicDesc.Pos.Y = 10.5469
	OBJ_131.PhysicDesc.Pos.Z = 505.25
	OBJ_131.PhysicDesc.Pos.Rotation = 270
	OBJ_131:SetPhysicalActorType(1)
	OBJ_131:SetPhysicalShape(5)
	OBJ_131.PhysicDesc.Collidable = true
	OBJ_131.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_25.phy"
	OBJ_131H = ActorHandler(OBJ_131)
	environment:AddActorObject(OBJ_131H)


	OBJ_132 = ActorObjectInfo(132)
	OBJ_132:SetRenderType(1)
	OBJ_132.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_25.osgb"
	OBJ_132.DisplayDesc.UseLight = false
	OBJ_132:SetModelState(1)
	OBJ_132.PhysicDesc.Pos.X = 486.5
	OBJ_132.PhysicDesc.Pos.Y = 10.1563
	OBJ_132.PhysicDesc.Pos.Z = 509.5
	OBJ_132.PhysicDesc.Pos.Rotation = 270
	OBJ_132:SetPhysicalActorType(1)
	OBJ_132:SetPhysicalShape(5)
	OBJ_132.PhysicDesc.Collidable = true
	OBJ_132.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_25.phy"
	OBJ_132H = ActorHandler(OBJ_132)
	environment:AddActorObject(OBJ_132H)


	OBJ_133 = ActorObjectInfo(133)
	OBJ_133:SetRenderType(1)
	OBJ_133.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_15.osgb"
	OBJ_133.DisplayDesc.UseLight = false
	OBJ_133:SetModelState(1)
	OBJ_133.PhysicDesc.Pos.X = 506
	OBJ_133.PhysicDesc.Pos.Y = 12.5
	OBJ_133.PhysicDesc.Pos.Z = 501
	OBJ_133.PhysicDesc.Pos.Rotation = 180
	OBJ_133:SetPhysicalActorType(1)
	OBJ_133:SetPhysicalShape(5)
	OBJ_133.PhysicDesc.Collidable = true
	OBJ_133.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_15.phy"
	OBJ_133H = ActorHandler(OBJ_133)
	environment:AddActorObject(OBJ_133H)


	OBJ_134 = ActorObjectInfo(134)
	OBJ_134:SetRenderType(1)
	OBJ_134.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_86.osgb"
	OBJ_134.DisplayDesc.UseLight = false
	OBJ_134:SetModelState(1)
	OBJ_134.PhysicDesc.Pos.X = 537
	OBJ_134.PhysicDesc.Pos.Y = 2.53906
	OBJ_134.PhysicDesc.Pos.Z = 490
	OBJ_134.PhysicDesc.Pos.Rotation = 90
	OBJ_134:SetPhysicalActorType(1)
	OBJ_134:SetPhysicalShape(5)
	OBJ_134.PhysicDesc.Collidable = true
	OBJ_134.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_86.phy"
	OBJ_134H = ActorHandler(OBJ_134)
	environment:AddActorObject(OBJ_134H)


	OBJ_135 = ActorObjectInfo(135)
	OBJ_135:SetRenderType(1)
	OBJ_135.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_87.osgb"
	OBJ_135.DisplayDesc.UseLight = false
	OBJ_135:SetModelState(1)
	OBJ_135.PhysicDesc.Pos.X = 531
	OBJ_135.PhysicDesc.Pos.Y = 3.125
	OBJ_135.PhysicDesc.Pos.Z = 487.002
	OBJ_135.PhysicDesc.Pos.Rotation = 90
	OBJ_135:SetPhysicalActorType(1)
	OBJ_135:SetPhysicalShape(5)
	OBJ_135.PhysicDesc.Collidable = true
	OBJ_135.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_87.phy"
	OBJ_135H = ActorHandler(OBJ_135)
	environment:AddActorObject(OBJ_135H)


	OBJ_136 = ActorObjectInfo(136)
	OBJ_136:SetRenderType(1)
	OBJ_136.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_88.osgb"
	OBJ_136.DisplayDesc.UseLight = false
	OBJ_136:SetModelState(1)
	OBJ_136.PhysicDesc.Pos.X = 523.5
	OBJ_136.PhysicDesc.Pos.Y = 12.3047
	OBJ_136.PhysicDesc.Pos.Z = 496
	OBJ_136.PhysicDesc.Pos.Rotation = 180
	OBJ_136:SetPhysicalActorType(1)
	OBJ_136:SetPhysicalShape(5)
	OBJ_136.PhysicDesc.Collidable = true
	OBJ_136.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_88.phy"
	OBJ_136H = ActorHandler(OBJ_136)
	environment:AddActorObject(OBJ_136H)


	OBJ_137 = ActorObjectInfo(137)
	OBJ_137:SetRenderType(1)
	OBJ_137.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_89.osgb"
	OBJ_137.DisplayDesc.UseLight = false
	OBJ_137:SetModelState(1)
	OBJ_137.PhysicDesc.Pos.X = 520.496
	OBJ_137.PhysicDesc.Pos.Y = 12.3047
	OBJ_137.PhysicDesc.Pos.Z = 499.875
	OBJ_137.PhysicDesc.Pos.Rotation = 180
	OBJ_137:SetPhysicalActorType(1)
	OBJ_137:SetPhysicalShape(5)
	OBJ_137.PhysicDesc.Collidable = true
	OBJ_137.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_89.phy"
	OBJ_137H = ActorHandler(OBJ_137)
	environment:AddActorObject(OBJ_137H)


	OBJ_138 = ActorObjectInfo(138)
	OBJ_138:SetRenderType(1)
	OBJ_138.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_89.osgb"
	OBJ_138.DisplayDesc.UseLight = false
	OBJ_138:SetModelState(1)
	OBJ_138.PhysicDesc.Pos.X = 520.496
	OBJ_138.PhysicDesc.Pos.Y = 12.3047
	OBJ_138.PhysicDesc.Pos.Z = 495.75
	OBJ_138.PhysicDesc.Pos.Rotation = 180
	OBJ_138:SetPhysicalActorType(1)
	OBJ_138:SetPhysicalShape(5)
	OBJ_138.PhysicDesc.Collidable = true
	OBJ_138.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_89.phy"
	OBJ_138H = ActorHandler(OBJ_138)
	environment:AddActorObject(OBJ_138H)


	OBJ_139 = ActorObjectInfo(139)
	OBJ_139:SetRenderType(1)
	OBJ_139.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_89.osgb"
	OBJ_139.DisplayDesc.UseLight = false
	OBJ_139:SetModelState(1)
	OBJ_139.PhysicDesc.Pos.X = 526.504
	OBJ_139.PhysicDesc.Pos.Y = 12.3047
	OBJ_139.PhysicDesc.Pos.Z = 499.875
	OBJ_139.PhysicDesc.Pos.Rotation = 360
	OBJ_139:SetPhysicalActorType(1)
	OBJ_139:SetPhysicalShape(5)
	OBJ_139.PhysicDesc.Collidable = true
	OBJ_139.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_89.phy"
	OBJ_139H = ActorHandler(OBJ_139)
	environment:AddActorObject(OBJ_139H)


	OBJ_140 = ActorObjectInfo(140)
	OBJ_140:SetRenderType(1)
	OBJ_140.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_89.osgb"
	OBJ_140.DisplayDesc.UseLight = false
	OBJ_140:SetModelState(1)
	OBJ_140.PhysicDesc.Pos.X = 526.504
	OBJ_140.PhysicDesc.Pos.Y = 12.2578
	OBJ_140.PhysicDesc.Pos.Z = 495.75
	OBJ_140.PhysicDesc.Pos.Rotation = 360
	OBJ_140:SetPhysicalActorType(1)
	OBJ_140:SetPhysicalShape(5)
	OBJ_140.PhysicDesc.Collidable = true
	OBJ_140.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_89.phy"
	OBJ_140H = ActorHandler(OBJ_140)
	environment:AddActorObject(OBJ_140H)


	OBJ_141 = ActorObjectInfo(141)
	OBJ_141:SetRenderType(1)
	OBJ_141.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_90.osgb"
	OBJ_141.DisplayDesc.UseLight = false
	OBJ_141:SetModelState(1)
	OBJ_141.PhysicDesc.Pos.X = 523.5
	OBJ_141.PhysicDesc.Pos.Y = 12.3047
	OBJ_141.PhysicDesc.Pos.Z = 496
	OBJ_141.PhysicDesc.Pos.Rotation = 180
	OBJ_141:SetPhysicalActorType(1)
	OBJ_141:SetPhysicalShape(5)
	OBJ_141.PhysicDesc.Collidable = true
	OBJ_141.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_90.phy"
	OBJ_141H = ActorHandler(OBJ_141)
	environment:AddActorObject(OBJ_141H)


	OBJ_142 = ActorObjectInfo(142)
	OBJ_142:SetRenderType(1)
	OBJ_142.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_91.osgb"
	OBJ_142.DisplayDesc.UseLight = false
	OBJ_142:SetModelState(1)
	OBJ_142.PhysicDesc.Pos.X = 523.5
	OBJ_142.PhysicDesc.Pos.Y = 12.3047
	OBJ_142.PhysicDesc.Pos.Z = 496
	OBJ_142.PhysicDesc.Pos.Rotation = 180
	OBJ_142:SetPhysicalActorType(1)
	OBJ_142:SetPhysicalShape(5)
	OBJ_142.PhysicDesc.Collidable = true
	OBJ_142.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_91.phy"
	OBJ_142H = ActorHandler(OBJ_142)
	environment:AddActorObject(OBJ_142H)


	OBJ_143 = ActorObjectInfo(143)
	OBJ_143:SetRenderType(1)
	OBJ_143.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_92.osgb"
	OBJ_143.DisplayDesc.UseLight = false
	OBJ_143:SetModelState(1)
	OBJ_143.PhysicDesc.Pos.X = 523.5
	OBJ_143.PhysicDesc.Pos.Y = 12.3047
	OBJ_143.PhysicDesc.Pos.Z = 496
	OBJ_143.PhysicDesc.Pos.Rotation = 180
	OBJ_143:SetPhysicalActorType(1)
	OBJ_143:SetPhysicalShape(5)
	OBJ_143.PhysicDesc.Collidable = true
	OBJ_143.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_92.phy"
	OBJ_143H = ActorHandler(OBJ_143)
	environment:AddActorObject(OBJ_143H)


	OBJ_144 = ActorObjectInfo(144)
	OBJ_144:SetRenderType(1)
	OBJ_144.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_93.osgb"
	OBJ_144.DisplayDesc.UseLight = false
	OBJ_144:SetModelState(1)
	OBJ_144.PhysicDesc.Pos.X = 523.5
	OBJ_144.PhysicDesc.Pos.Y = 12.3047
	OBJ_144.PhysicDesc.Pos.Z = 496
	OBJ_144.PhysicDesc.Pos.Rotation = 180
	OBJ_144:SetPhysicalActorType(1)
	OBJ_144:SetPhysicalShape(5)
	OBJ_144.PhysicDesc.Collidable = true
	OBJ_144.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_93.phy"
	OBJ_144H = ActorHandler(OBJ_144)
	environment:AddActorObject(OBJ_144H)


	OBJ_145 = ActorObjectInfo(145)
	OBJ_145:SetRenderType(1)
	OBJ_145.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_94.osgb"
	OBJ_145.DisplayDesc.UseLight = false
	OBJ_145:SetModelState(1)
	OBJ_145.PhysicDesc.Pos.X = 523.5
	OBJ_145.PhysicDesc.Pos.Y = 12.3047
	OBJ_145.PhysicDesc.Pos.Z = 496
	OBJ_145.PhysicDesc.Pos.Rotation = 180
	OBJ_145:SetPhysicalActorType(1)
	OBJ_145:SetPhysicalShape(5)
	OBJ_145.PhysicDesc.Collidable = true
	OBJ_145.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_94.phy"
	OBJ_145H = ActorHandler(OBJ_145)
	environment:AddActorObject(OBJ_145H)


	OBJ_146 = ActorObjectInfo(146)
	OBJ_146:SetRenderType(1)
	OBJ_146.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_23.osgb"
	OBJ_146.DisplayDesc.UseLight = false
	OBJ_146:SetModelState(1)
	OBJ_146.PhysicDesc.Pos.X = 495.25
	OBJ_146.PhysicDesc.Pos.Y = 10.5469
	OBJ_146.PhysicDesc.Pos.Z = 498.5
	OBJ_146.PhysicDesc.Pos.Rotation = 180
	OBJ_146:SetPhysicalActorType(1)
	OBJ_146:SetPhysicalShape(5)
	OBJ_146.PhysicDesc.Collidable = true
	OBJ_146.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_23.phy"
	OBJ_146H = ActorHandler(OBJ_146)
	environment:AddActorObject(OBJ_146H)


	OBJ_147 = ActorObjectInfo(147)
	OBJ_147:SetRenderType(1)
	OBJ_147.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_23.osgb"
	OBJ_147.DisplayDesc.UseLight = false
	OBJ_147:SetModelState(1)
	OBJ_147.PhysicDesc.Pos.X = 492.375
	OBJ_147.PhysicDesc.Pos.Y = 10.5469
	OBJ_147.PhysicDesc.Pos.Z = 501.5
	OBJ_147.PhysicDesc.Pos.Rotation = 270
	OBJ_147:SetPhysicalActorType(1)
	OBJ_147:SetPhysicalShape(5)
	OBJ_147.PhysicDesc.Collidable = true
	OBJ_147.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_23.phy"
	OBJ_147H = ActorHandler(OBJ_147)
	environment:AddActorObject(OBJ_147H)


	OBJ_148 = ActorObjectInfo(148)
	OBJ_148:SetRenderType(1)
	OBJ_148.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_95.osgb"
	OBJ_148.DisplayDesc.UseLight = false
	OBJ_148:SetModelState(1)
	OBJ_148.PhysicDesc.Pos.X = 497.25
	OBJ_148.PhysicDesc.Pos.Y = 10.5469
	OBJ_148.PhysicDesc.Pos.Z = 502
	OBJ_148.PhysicDesc.Pos.Rotation = 180
	OBJ_148:SetPhysicalActorType(1)
	OBJ_148:SetPhysicalShape(5)
	OBJ_148.PhysicDesc.Collidable = true
	OBJ_148.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_95.phy"
	OBJ_148H = ActorHandler(OBJ_148)
	environment:AddActorObject(OBJ_148H)


	OBJ_149 = ActorObjectInfo(149)
	OBJ_149:SetRenderType(1)
	OBJ_149.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_96.osgb"
	OBJ_149.DisplayDesc.UseLight = false
	OBJ_149:SetModelState(1)
	OBJ_149.PhysicDesc.Pos.X = 497.25
	OBJ_149.PhysicDesc.Pos.Y = 10.5469
	OBJ_149.PhysicDesc.Pos.Z = 502
	OBJ_149.PhysicDesc.Pos.Rotation = 180
	OBJ_149:SetPhysicalActorType(1)
	OBJ_149:SetPhysicalShape(5)
	OBJ_149.PhysicDesc.Collidable = true
	OBJ_149.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_96.phy"
	OBJ_149H = ActorHandler(OBJ_149)
	environment:AddActorObject(OBJ_149H)


	OBJ_150 = ActorObjectInfo(150)
	OBJ_150:SetRenderType(1)
	OBJ_150.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_97.osgb"
	OBJ_150.DisplayDesc.UseLight = false
	OBJ_150:SetModelState(1)
	OBJ_150.PhysicDesc.Pos.X = 497.25
	OBJ_150.PhysicDesc.Pos.Y = 10.5469
	OBJ_150.PhysicDesc.Pos.Z = 502
	OBJ_150.PhysicDesc.Pos.Rotation = 180
	OBJ_150:SetPhysicalActorType(1)
	OBJ_150:SetPhysicalShape(5)
	OBJ_150.PhysicDesc.Collidable = true
	OBJ_150.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_97.phy"
	OBJ_150H = ActorHandler(OBJ_150)
	environment:AddActorObject(OBJ_150H)


	OBJ_151 = ActorObjectInfo(151)
	OBJ_151:SetRenderType(1)
	OBJ_151.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_98.osgb"
	OBJ_151.DisplayDesc.UseLight = false
	OBJ_151:SetModelState(1)
	OBJ_151.PhysicDesc.Pos.X = 497.25
	OBJ_151.PhysicDesc.Pos.Y = 10.5469
	OBJ_151.PhysicDesc.Pos.Z = 502
	OBJ_151.PhysicDesc.Pos.Rotation = 180
	OBJ_151:SetPhysicalActorType(1)
	OBJ_151:SetPhysicalShape(5)
	OBJ_151.PhysicDesc.Collidable = true
	OBJ_151.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_98.phy"
	OBJ_151H = ActorHandler(OBJ_151)
	environment:AddActorObject(OBJ_151H)


	OBJ_152 = ActorObjectInfo(152)
	OBJ_152:SetRenderType(1)
	OBJ_152.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_99.osgb"
	OBJ_152.DisplayDesc.UseLight = false
	OBJ_152:SetModelState(1)
	OBJ_152.PhysicDesc.Pos.X = 500.004
	OBJ_152.PhysicDesc.Pos.Y = 11.1328
	OBJ_152.PhysicDesc.Pos.Z = 505
	OBJ_152.PhysicDesc.Pos.Rotation = 90
	OBJ_152:SetPhysicalActorType(1)
	OBJ_152:SetPhysicalShape(5)
	OBJ_152.PhysicDesc.Collidable = true
	OBJ_152.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_99.phy"
	OBJ_152H = ActorHandler(OBJ_152)
	environment:AddActorObject(OBJ_152H)


	OBJ_153 = ActorObjectInfo(153)
	OBJ_153:SetRenderType(1)
	OBJ_153.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_100.osgb"
	OBJ_153.DisplayDesc.UseLight = false
	OBJ_153:SetModelState(1)
	OBJ_153.PhysicDesc.Pos.X = 500.004
	OBJ_153.PhysicDesc.Pos.Y = 11.1328
	OBJ_153.PhysicDesc.Pos.Z = 505
	OBJ_153.PhysicDesc.Pos.Rotation = 90
	OBJ_153:SetPhysicalActorType(1)
	OBJ_153:SetPhysicalShape(5)
	OBJ_153.PhysicDesc.Collidable = true
	OBJ_153.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_100.phy"
	OBJ_153H = ActorHandler(OBJ_153)
	environment:AddActorObject(OBJ_153H)


	OBJ_154 = ActorObjectInfo(154)
	OBJ_154:SetRenderType(1)
	OBJ_154.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_101.osgb"
	OBJ_154.DisplayDesc.UseLight = false
	OBJ_154:SetModelState(1)
	OBJ_154.PhysicDesc.Pos.X = 489.582
	OBJ_154.PhysicDesc.Pos.Y = 10.3828
	OBJ_154.PhysicDesc.Pos.Z = 511.195
	OBJ_154.PhysicDesc.Pos.Rotation = 90
	OBJ_154:SetPhysicalActorType(1)
	OBJ_154:SetPhysicalShape(5)
	OBJ_154.PhysicDesc.Collidable = true
	OBJ_154.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_101.phy"
	OBJ_154H = ActorHandler(OBJ_154)
	environment:AddActorObject(OBJ_154H)


	OBJ_155 = ActorObjectInfo(155)
	OBJ_155:SetRenderType(1)
	OBJ_155.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_101.osgb"
	OBJ_155.DisplayDesc.UseLight = false
	OBJ_155:SetModelState(1)
	OBJ_155.PhysicDesc.Pos.X = 489.582
	OBJ_155.PhysicDesc.Pos.Y = 10.3828
	OBJ_155.PhysicDesc.Pos.Z = 513.09
	OBJ_155.PhysicDesc.Pos.Rotation = 90
	OBJ_155:SetPhysicalActorType(1)
	OBJ_155:SetPhysicalShape(5)
	OBJ_155.PhysicDesc.Collidable = true
	OBJ_155.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_101.phy"
	OBJ_155H = ActorHandler(OBJ_155)
	environment:AddActorObject(OBJ_155H)


	OBJ_156 = ActorObjectInfo(156)
	OBJ_156:SetRenderType(1)
	OBJ_156.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_102.osgb"
	OBJ_156.DisplayDesc.UseLight = false
	OBJ_156:SetModelState(1)
	OBJ_156.PhysicDesc.Pos.X = 491.625
	OBJ_156.PhysicDesc.Pos.Y = 10.5469
	OBJ_156.PhysicDesc.Pos.Z = 506.75
	OBJ_156.PhysicDesc.Pos.Rotation = 270
	OBJ_156:SetPhysicalActorType(1)
	OBJ_156:SetPhysicalShape(5)
	OBJ_156.PhysicDesc.Collidable = true
	OBJ_156.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_102.phy"
	OBJ_156H = ActorHandler(OBJ_156)
	environment:AddActorObject(OBJ_156H)


	OBJ_157 = ActorObjectInfo(157)
	OBJ_157:SetRenderType(1)
	OBJ_157.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_103.osgb"
	OBJ_157.DisplayDesc.UseLight = false
	OBJ_157:SetModelState(1)
	OBJ_157.PhysicDesc.Pos.X = 493.5
	OBJ_157.PhysicDesc.Pos.Y = 10.5469
	OBJ_157.PhysicDesc.Pos.Z = 507.625
	OBJ_157.PhysicDesc.Pos.Rotation = 270
	OBJ_157:SetPhysicalActorType(1)
	OBJ_157:SetPhysicalShape(5)
	OBJ_157.PhysicDesc.Collidable = true
	OBJ_157.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_103.phy"
	OBJ_157H = ActorHandler(OBJ_157)
	environment:AddActorObject(OBJ_157H)


	OBJ_158 = ActorObjectInfo(158)
	OBJ_158:SetRenderType(1)
	OBJ_158.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_104.osgb"
	OBJ_158.DisplayDesc.UseLight = false
	OBJ_158:SetModelState(1)
	OBJ_158.PhysicDesc.Pos.X = 494.5
	OBJ_158.PhysicDesc.Pos.Y = 11.1328
	OBJ_158.PhysicDesc.Pos.Z = 490.5
	OBJ_158.PhysicDesc.Pos.Rotation = 180
	OBJ_158:SetPhysicalActorType(1)
	OBJ_158:SetPhysicalShape(5)
	OBJ_158.PhysicDesc.Collidable = true
	OBJ_158.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_104.phy"
	OBJ_158H = ActorHandler(OBJ_158)
	environment:AddActorObject(OBJ_158H)


	OBJ_159 = ActorObjectInfo(159)
	OBJ_159:SetRenderType(1)
	OBJ_159.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_104.osgb"
	OBJ_159.DisplayDesc.UseLight = false
	OBJ_159:SetModelState(1)
	OBJ_159.PhysicDesc.Pos.X = 492.5
	OBJ_159.PhysicDesc.Pos.Y = 10.8398
	OBJ_159.PhysicDesc.Pos.Z = 488.5
	OBJ_159.PhysicDesc.Pos.Rotation = 180
	OBJ_159:SetPhysicalActorType(1)
	OBJ_159:SetPhysicalShape(5)
	OBJ_159.PhysicDesc.Collidable = true
	OBJ_159.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_104.phy"
	OBJ_159H = ActorHandler(OBJ_159)
	environment:AddActorObject(OBJ_159H)


	OBJ_160 = ActorObjectInfo(160)
	OBJ_160:SetRenderType(1)
	OBJ_160.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_104.osgb"
	OBJ_160.DisplayDesc.UseLight = false
	OBJ_160:SetModelState(1)
	OBJ_160.PhysicDesc.Pos.X = 492.5
	OBJ_160.PhysicDesc.Pos.Y = 11.1328
	OBJ_160.PhysicDesc.Pos.Z = 490.5
	OBJ_160.PhysicDesc.Pos.Rotation = 180
	OBJ_160:SetPhysicalActorType(1)
	OBJ_160:SetPhysicalShape(5)
	OBJ_160.PhysicDesc.Collidable = true
	OBJ_160.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_104.phy"
	OBJ_160H = ActorHandler(OBJ_160)
	environment:AddActorObject(OBJ_160H)


	OBJ_161 = ActorObjectInfo(161)
	OBJ_161:SetRenderType(1)
	OBJ_161.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_105.osgb"
	OBJ_161.DisplayDesc.UseLight = false
	OBJ_161:SetModelState(1)
	OBJ_161.PhysicDesc.Pos.X = 490.5
	OBJ_161.PhysicDesc.Pos.Y = 10.7656
	OBJ_161.PhysicDesc.Pos.Z = 489.875
	OBJ_161.PhysicDesc.Pos.Rotation = 270
	OBJ_161:SetPhysicalActorType(1)
	OBJ_161:SetPhysicalShape(5)
	OBJ_161.PhysicDesc.Collidable = true
	OBJ_161.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_105.phy"
	OBJ_161H = ActorHandler(OBJ_161)
	environment:AddActorObject(OBJ_161H)


	OBJ_162 = ActorObjectInfo(162)
	OBJ_162:SetRenderType(1)
	OBJ_162.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_102.osgb"
	OBJ_162.DisplayDesc.UseLight = false
	OBJ_162:SetModelState(1)
	OBJ_162.PhysicDesc.Pos.X = 487.5
	OBJ_162.PhysicDesc.Pos.Y = 10.8398
	OBJ_162.PhysicDesc.Pos.Z = 492.5
	OBJ_162.PhysicDesc.Pos.Rotation = 360
	OBJ_162:SetPhysicalActorType(1)
	OBJ_162:SetPhysicalShape(5)
	OBJ_162.PhysicDesc.Collidable = true
	OBJ_162.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_102.phy"
	OBJ_162H = ActorHandler(OBJ_162)
	environment:AddActorObject(OBJ_162H)


	OBJ_163 = ActorObjectInfo(163)
	OBJ_163:SetRenderType(1)
	OBJ_163.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_106.osgb"
	OBJ_163.DisplayDesc.UseLight = false
	OBJ_163:SetModelState(1)
	OBJ_163.PhysicDesc.Pos.X = 519
	OBJ_163.PhysicDesc.Pos.Y = 11.3281
	OBJ_163.PhysicDesc.Pos.Z = 495.5
	OBJ_163.PhysicDesc.Pos.Rotation = 90
	OBJ_163:SetPhysicalActorType(1)
	OBJ_163:SetPhysicalShape(5)
	OBJ_163.PhysicDesc.Collidable = true
	OBJ_163.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_106.phy"
	OBJ_163H = ActorHandler(OBJ_163)
	environment:AddActorObject(OBJ_163H)


	OBJ_164 = ActorObjectInfo(164)
	OBJ_164:SetRenderType(1)
	OBJ_164.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_104.osgb"
	OBJ_164.DisplayDesc.UseLight = false
	OBJ_164:SetModelState(1)
	OBJ_164.PhysicDesc.Pos.X = 495.5
	OBJ_164.PhysicDesc.Pos.Y = 11.1328
	OBJ_164.PhysicDesc.Pos.Z = 536.5
	OBJ_164.PhysicDesc.Pos.Rotation = 180
	OBJ_164:SetPhysicalActorType(1)
	OBJ_164:SetPhysicalShape(5)
	OBJ_164.PhysicDesc.Collidable = true
	OBJ_164.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_104.phy"
	OBJ_164H = ActorHandler(OBJ_164)
	environment:AddActorObject(OBJ_164H)


	OBJ_165 = ActorObjectInfo(165)
	OBJ_165:SetRenderType(1)
	OBJ_165.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_104.osgb"
	OBJ_165.DisplayDesc.UseLight = false
	OBJ_165:SetModelState(1)
	OBJ_165.PhysicDesc.Pos.X = 497.5
	OBJ_165.PhysicDesc.Pos.Y = 11.1328
	OBJ_165.PhysicDesc.Pos.Z = 534.5
	OBJ_165.PhysicDesc.Pos.Rotation = 180
	OBJ_165:SetPhysicalActorType(1)
	OBJ_165:SetPhysicalShape(5)
	OBJ_165.PhysicDesc.Collidable = true
	OBJ_165.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_104.phy"
	OBJ_165H = ActorHandler(OBJ_165)
	environment:AddActorObject(OBJ_165H)


	OBJ_166 = ActorObjectInfo(166)
	OBJ_166:SetRenderType(1)
	OBJ_166.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_105.osgb"
	OBJ_166.DisplayDesc.UseLight = false
	OBJ_166:SetModelState(1)
	OBJ_166.PhysicDesc.Pos.X = 494.5
	OBJ_166.PhysicDesc.Pos.Y = 11.1328
	OBJ_166.PhysicDesc.Pos.Z = 534.5
	OBJ_166.PhysicDesc.Pos.Rotation = 180
	OBJ_166:SetPhysicalActorType(1)
	OBJ_166:SetPhysicalShape(5)
	OBJ_166.PhysicDesc.Collidable = true
	OBJ_166.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_105.phy"
	OBJ_166H = ActorHandler(OBJ_166)
	environment:AddActorObject(OBJ_166H)


	OBJ_167 = ActorObjectInfo(167)
	OBJ_167:SetRenderType(1)
	OBJ_167.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_25.osgb"
	OBJ_167.DisplayDesc.UseLight = false
	OBJ_167:SetModelState(1)
	OBJ_167.PhysicDesc.Pos.X = 486.5
	OBJ_167.PhysicDesc.Pos.Y = 10.1563
	OBJ_167.PhysicDesc.Pos.Z = 515.5
	OBJ_167.PhysicDesc.Pos.Rotation = 270
	OBJ_167:SetPhysicalActorType(1)
	OBJ_167:SetPhysicalShape(5)
	OBJ_167.PhysicDesc.Collidable = true
	OBJ_167.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_25.phy"
	OBJ_167H = ActorHandler(OBJ_167)
	environment:AddActorObject(OBJ_167H)


	OBJ_168 = ActorObjectInfo(168)
	OBJ_168:SetRenderType(1)
	OBJ_168.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_33.osgb"
	OBJ_168.DisplayDesc.UseLight = false
	OBJ_168:SetModelState(1)
	OBJ_168.PhysicDesc.Pos.X = 504.5
	OBJ_168.PhysicDesc.Pos.Y = 10.5469
	OBJ_168.PhysicDesc.Pos.Z = 498.5
	OBJ_168.PhysicDesc.Pos.Rotation = 180
	OBJ_168:SetPhysicalActorType(1)
	OBJ_168:SetPhysicalShape(5)
	OBJ_168.PhysicDesc.Collidable = true
	OBJ_168.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_33.phy"
	OBJ_168H = ActorHandler(OBJ_168)
	environment:AddActorObject(OBJ_168H)


	OBJ_169 = ActorObjectInfo(169)
	OBJ_169:SetRenderType(1)
	OBJ_169.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_34.osgb"
	OBJ_169.DisplayDesc.UseLight = false
	OBJ_169:SetModelState(1)
	OBJ_169.PhysicDesc.Pos.X = 501.5
	OBJ_169.PhysicDesc.Pos.Y = 11.1328
	OBJ_169.PhysicDesc.Pos.Z = 490.5
	OBJ_169.PhysicDesc.Pos.Rotation = 270
	OBJ_169:SetPhysicalActorType(1)
	OBJ_169:SetPhysicalShape(5)
	OBJ_169.PhysicDesc.Collidable = true
	OBJ_169.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/PLATFORM_OBJ_34.phy"
	OBJ_169H = ActorHandler(OBJ_169)
	environment:AddActorObject(OBJ_169H)
end
