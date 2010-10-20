function InitMap(environment)
	MapObject = ActorObjectInfo(1)
	MapObject:SetRenderType(1)
	MapObject.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB.osgb"
	MapObject.DisplayDesc.UseLight = false
	MapObject:SetModelState(1)
	MapObject.PhysicDesc.Pos.X = 0
	MapObject.PhysicDesc.Pos.Y = 0
	MapObject.PhysicDesc.Pos.Z = 0
	MapObject.PhysicDesc.Pos.Rotation = 0
	MapObject:SetPhysicalActorType(1)
	MapObject:SetPhysicalShape(5)
	MapObject.PhysicDesc.Collidable = true
	MapObject.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB.phy"
	MapObjectH = ActorHandler(MapObject)
	environment:AddActorObject(MapObjectH)


	OBJ_2 = ActorObjectInfo(2)
	OBJ_2:SetRenderType(1)
	OBJ_2.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_1.osgb"
	OBJ_2.DisplayDesc.UseLight = false
	OBJ_2:SetModelState(1)
	OBJ_2.PhysicDesc.Pos.X = 437
	OBJ_2.PhysicDesc.Pos.Y = 23.0469
	OBJ_2.PhysicDesc.Pos.Z = 528
	OBJ_2.PhysicDesc.Pos.Rotation = 90
	OBJ_2:SetPhysicalActorType(1)
	OBJ_2:SetPhysicalShape(5)
	OBJ_2.PhysicDesc.Collidable = true
	OBJ_2.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_1.phy"
	OBJ_2H = ActorHandler(OBJ_2)
	environment:AddActorObject(OBJ_2H)


	OBJ_3 = ActorObjectInfo(3)
	OBJ_3:SetRenderType(1)
	OBJ_3.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_2.osgb"
	OBJ_3.DisplayDesc.UseLight = false
	OBJ_3:SetModelState(1)
	OBJ_3.PhysicDesc.Pos.X = 461.5
	OBJ_3.PhysicDesc.Pos.Y = 19.1406
	OBJ_3.PhysicDesc.Pos.Z = 522.5
	OBJ_3.PhysicDesc.Pos.Rotation = 180
	OBJ_3:SetPhysicalActorType(1)
	OBJ_3:SetPhysicalShape(5)
	OBJ_3.PhysicDesc.Collidable = true
	OBJ_3.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_2.phy"
	OBJ_3H = ActorHandler(OBJ_3)
	environment:AddActorObject(OBJ_3H)


	OBJ_4 = ActorObjectInfo(4)
	OBJ_4:SetRenderType(1)
	OBJ_4.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_2.osgb"
	OBJ_4.DisplayDesc.UseLight = false
	OBJ_4:SetModelState(1)
	OBJ_4.PhysicDesc.Pos.X = 473.625
	OBJ_4.PhysicDesc.Pos.Y = 17.7461
	OBJ_4.PhysicDesc.Pos.Z = 514.375
	OBJ_4.PhysicDesc.Pos.Rotation = 360
	OBJ_4:SetPhysicalActorType(1)
	OBJ_4:SetPhysicalShape(5)
	OBJ_4.PhysicDesc.Collidable = true
	OBJ_4.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_2.phy"
	OBJ_4H = ActorHandler(OBJ_4)
	environment:AddActorObject(OBJ_4H)


	OBJ_5 = ActorObjectInfo(5)
	OBJ_5:SetRenderType(1)
	OBJ_5.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_3.osgb"
	OBJ_5.DisplayDesc.UseLight = false
	OBJ_5:SetModelState(1)
	OBJ_5.PhysicDesc.Pos.X = 465.375
	OBJ_5.PhysicDesc.Pos.Y = 26.3672
	OBJ_5.PhysicDesc.Pos.Z = 533.375
	OBJ_5.PhysicDesc.Pos.Rotation = 90
	OBJ_5:SetPhysicalActorType(1)
	OBJ_5:SetPhysicalShape(5)
	OBJ_5.PhysicDesc.Collidable = true
	OBJ_5.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_3.phy"
	OBJ_5H = ActorHandler(OBJ_5)
	environment:AddActorObject(OBJ_5H)


	OBJ_6 = ActorObjectInfo(6)
	OBJ_6:SetRenderType(1)
	OBJ_6.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_4.osgb"
	OBJ_6.DisplayDesc.UseLight = false
	OBJ_6:SetModelState(1)
	OBJ_6.PhysicDesc.Pos.X = 471.5
	OBJ_6.PhysicDesc.Pos.Y = 17.3828
	OBJ_6.PhysicDesc.Pos.Z = 493.5
	OBJ_6.PhysicDesc.Pos.Rotation = 270
	OBJ_6:SetPhysicalActorType(1)
	OBJ_6:SetPhysicalShape(5)
	OBJ_6.PhysicDesc.Collidable = true
	OBJ_6.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_4.phy"
	OBJ_6H = ActorHandler(OBJ_6)
	environment:AddActorObject(OBJ_6H)


	OBJ_7 = ActorObjectInfo(7)
	OBJ_7:SetRenderType(1)
	OBJ_7.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_5.osgb"
	OBJ_7.DisplayDesc.UseLight = false
	OBJ_7:SetModelState(1)
	OBJ_7.PhysicDesc.Pos.X = 471.5
	OBJ_7.PhysicDesc.Pos.Y = 17.3828
	OBJ_7.PhysicDesc.Pos.Z = 493.5
	OBJ_7.PhysicDesc.Pos.Rotation = 270
	OBJ_7:SetPhysicalActorType(1)
	OBJ_7:SetPhysicalShape(5)
	OBJ_7.PhysicDesc.Collidable = true
	OBJ_7.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_5.phy"
	OBJ_7H = ActorHandler(OBJ_7)
	environment:AddActorObject(OBJ_7H)


	OBJ_8 = ActorObjectInfo(8)
	OBJ_8:SetRenderType(1)
	OBJ_8.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_6.osgb"
	OBJ_8.DisplayDesc.UseLight = false
	OBJ_8:SetModelState(1)
	OBJ_8.PhysicDesc.Pos.X = 471.5
	OBJ_8.PhysicDesc.Pos.Y = 17.3828
	OBJ_8.PhysicDesc.Pos.Z = 493.5
	OBJ_8.PhysicDesc.Pos.Rotation = 270
	OBJ_8:SetPhysicalActorType(1)
	OBJ_8:SetPhysicalShape(5)
	OBJ_8.PhysicDesc.Collidable = true
	OBJ_8.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_6.phy"
	OBJ_8H = ActorHandler(OBJ_8)
	environment:AddActorObject(OBJ_8H)


	OBJ_9 = ActorObjectInfo(9)
	OBJ_9:SetRenderType(1)
	OBJ_9.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_7.osgb"
	OBJ_9.DisplayDesc.UseLight = false
	OBJ_9:SetModelState(1)
	OBJ_9.PhysicDesc.Pos.X = 471.5
	OBJ_9.PhysicDesc.Pos.Y = 17.3828
	OBJ_9.PhysicDesc.Pos.Z = 493.5
	OBJ_9.PhysicDesc.Pos.Rotation = 270
	OBJ_9:SetPhysicalActorType(1)
	OBJ_9:SetPhysicalShape(5)
	OBJ_9.PhysicDesc.Collidable = true
	OBJ_9.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_7.phy"
	OBJ_9H = ActorHandler(OBJ_9)
	environment:AddActorObject(OBJ_9H)


	OBJ_10 = ActorObjectInfo(10)
	OBJ_10:SetRenderType(1)
	OBJ_10.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_8.osgb"
	OBJ_10.DisplayDesc.UseLight = false
	OBJ_10:SetModelState(1)
	OBJ_10.PhysicDesc.Pos.X = 471.5
	OBJ_10.PhysicDesc.Pos.Y = 17.3828
	OBJ_10.PhysicDesc.Pos.Z = 493.5
	OBJ_10.PhysicDesc.Pos.Rotation = 270
	OBJ_10:SetPhysicalActorType(1)
	OBJ_10:SetPhysicalShape(5)
	OBJ_10.PhysicDesc.Collidable = true
	OBJ_10.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_8.phy"
	OBJ_10H = ActorHandler(OBJ_10)
	environment:AddActorObject(OBJ_10H)


	OBJ_11 = ActorObjectInfo(11)
	OBJ_11:SetRenderType(1)
	OBJ_11.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_9.osgb"
	OBJ_11.DisplayDesc.UseLight = false
	OBJ_11:SetModelState(1)
	OBJ_11.PhysicDesc.Pos.X = 471.5
	OBJ_11.PhysicDesc.Pos.Y = 17.3828
	OBJ_11.PhysicDesc.Pos.Z = 493.5
	OBJ_11.PhysicDesc.Pos.Rotation = 270
	OBJ_11:SetPhysicalActorType(1)
	OBJ_11:SetPhysicalShape(5)
	OBJ_11.PhysicDesc.Collidable = true
	OBJ_11.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_9.phy"
	OBJ_11H = ActorHandler(OBJ_11)
	environment:AddActorObject(OBJ_11H)


	OBJ_12 = ActorObjectInfo(12)
	OBJ_12:SetRenderType(1)
	OBJ_12.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_2.osgb"
	OBJ_12.DisplayDesc.UseLight = false
	OBJ_12:SetModelState(1)
	OBJ_12.PhysicDesc.Pos.X = 478.25
	OBJ_12.PhysicDesc.Pos.Y = 17.9688
	OBJ_12.PhysicDesc.Pos.Z = 520.25
	OBJ_12.PhysicDesc.Pos.Rotation = 180
	OBJ_12:SetPhysicalActorType(1)
	OBJ_12:SetPhysicalShape(5)
	OBJ_12.PhysicDesc.Collidable = true
	OBJ_12.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_2.phy"
	OBJ_12H = ActorHandler(OBJ_12)
	environment:AddActorObject(OBJ_12H)


	OBJ_13 = ActorObjectInfo(13)
	OBJ_13:SetRenderType(1)
	OBJ_13.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_3.osgb"
	OBJ_13.DisplayDesc.UseLight = false
	OBJ_13:SetModelState(1)
	OBJ_13.PhysicDesc.Pos.X = 478.5
	OBJ_13.PhysicDesc.Pos.Y = 17.9688
	OBJ_13.PhysicDesc.Pos.Z = 515.5
	OBJ_13.PhysicDesc.Pos.Rotation = 180
	OBJ_13:SetPhysicalActorType(1)
	OBJ_13:SetPhysicalShape(5)
	OBJ_13.PhysicDesc.Collidable = true
	OBJ_13.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_3.phy"
	OBJ_13H = ActorHandler(OBJ_13)
	environment:AddActorObject(OBJ_13H)


	OBJ_14 = ActorObjectInfo(14)
	OBJ_14:SetRenderType(1)
	OBJ_14.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_10.osgb"
	OBJ_14.DisplayDesc.UseLight = false
	OBJ_14:SetModelState(1)
	OBJ_14.PhysicDesc.Pos.X = 465
	OBJ_14.PhysicDesc.Pos.Y = 3.32031
	OBJ_14.PhysicDesc.Pos.Z = 510
	OBJ_14.PhysicDesc.Pos.Rotation = 90
	OBJ_14:SetPhysicalActorType(1)
	OBJ_14:SetPhysicalShape(5)
	OBJ_14.PhysicDesc.Collidable = true
	OBJ_14.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_10.phy"
	OBJ_14H = ActorHandler(OBJ_14)
	environment:AddActorObject(OBJ_14H)


	OBJ_15 = ActorObjectInfo(15)
	OBJ_15:SetRenderType(1)
	OBJ_15.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_11.osgb"
	OBJ_15.DisplayDesc.UseLight = false
	OBJ_15:SetModelState(1)
	OBJ_15.PhysicDesc.Pos.X = 465
	OBJ_15.PhysicDesc.Pos.Y = 6.09766
	OBJ_15.PhysicDesc.Pos.Z = 510.25
	OBJ_15.PhysicDesc.Pos.Rotation = 90
	OBJ_15:SetPhysicalActorType(1)
	OBJ_15:SetPhysicalShape(5)
	OBJ_15.PhysicDesc.Collidable = true
	OBJ_15.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_11.phy"
	OBJ_15H = ActorHandler(OBJ_15)
	environment:AddActorObject(OBJ_15H)


	OBJ_16 = ActorObjectInfo(16)
	OBJ_16:SetRenderType(1)
	OBJ_16.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_4.osgb"
	OBJ_16.DisplayDesc.UseLight = false
	OBJ_16:SetModelState(1)
	OBJ_16.PhysicDesc.Pos.X = 466.5
	OBJ_16.PhysicDesc.Pos.Y = 17.9688
	OBJ_16.PhysicDesc.Pos.Z = 517.5
	OBJ_16.PhysicDesc.Pos.Rotation = 270
	OBJ_16:SetPhysicalActorType(1)
	OBJ_16:SetPhysicalShape(5)
	OBJ_16.PhysicDesc.Collidable = true
	OBJ_16.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_4.phy"
	OBJ_16H = ActorHandler(OBJ_16)
	environment:AddActorObject(OBJ_16H)


	OBJ_17 = ActorObjectInfo(17)
	OBJ_17:SetRenderType(1)
	OBJ_17.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_5.osgb"
	OBJ_17.DisplayDesc.UseLight = false
	OBJ_17:SetModelState(1)
	OBJ_17.PhysicDesc.Pos.X = 466.5
	OBJ_17.PhysicDesc.Pos.Y = 17.9688
	OBJ_17.PhysicDesc.Pos.Z = 517.5
	OBJ_17.PhysicDesc.Pos.Rotation = 270
	OBJ_17:SetPhysicalActorType(1)
	OBJ_17:SetPhysicalShape(5)
	OBJ_17.PhysicDesc.Collidable = true
	OBJ_17.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_5.phy"
	OBJ_17H = ActorHandler(OBJ_17)
	environment:AddActorObject(OBJ_17H)


	OBJ_18 = ActorObjectInfo(18)
	OBJ_18:SetRenderType(1)
	OBJ_18.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_6.osgb"
	OBJ_18.DisplayDesc.UseLight = false
	OBJ_18:SetModelState(1)
	OBJ_18.PhysicDesc.Pos.X = 466.5
	OBJ_18.PhysicDesc.Pos.Y = 17.9688
	OBJ_18.PhysicDesc.Pos.Z = 517.5
	OBJ_18.PhysicDesc.Pos.Rotation = 270
	OBJ_18:SetPhysicalActorType(1)
	OBJ_18:SetPhysicalShape(5)
	OBJ_18.PhysicDesc.Collidable = true
	OBJ_18.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_6.phy"
	OBJ_18H = ActorHandler(OBJ_18)
	environment:AddActorObject(OBJ_18H)


	OBJ_19 = ActorObjectInfo(19)
	OBJ_19:SetRenderType(1)
	OBJ_19.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_7.osgb"
	OBJ_19.DisplayDesc.UseLight = false
	OBJ_19:SetModelState(1)
	OBJ_19.PhysicDesc.Pos.X = 466.5
	OBJ_19.PhysicDesc.Pos.Y = 17.9688
	OBJ_19.PhysicDesc.Pos.Z = 517.5
	OBJ_19.PhysicDesc.Pos.Rotation = 270
	OBJ_19:SetPhysicalActorType(1)
	OBJ_19:SetPhysicalShape(5)
	OBJ_19.PhysicDesc.Collidable = true
	OBJ_19.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_7.phy"
	OBJ_19H = ActorHandler(OBJ_19)
	environment:AddActorObject(OBJ_19H)


	OBJ_20 = ActorObjectInfo(20)
	OBJ_20:SetRenderType(1)
	OBJ_20.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_8.osgb"
	OBJ_20.DisplayDesc.UseLight = false
	OBJ_20:SetModelState(1)
	OBJ_20.PhysicDesc.Pos.X = 466.5
	OBJ_20.PhysicDesc.Pos.Y = 17.9688
	OBJ_20.PhysicDesc.Pos.Z = 517.5
	OBJ_20.PhysicDesc.Pos.Rotation = 270
	OBJ_20:SetPhysicalActorType(1)
	OBJ_20:SetPhysicalShape(5)
	OBJ_20.PhysicDesc.Collidable = true
	OBJ_20.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_8.phy"
	OBJ_20H = ActorHandler(OBJ_20)
	environment:AddActorObject(OBJ_20H)


	OBJ_21 = ActorObjectInfo(21)
	OBJ_21:SetRenderType(1)
	OBJ_21.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_9.osgb"
	OBJ_21.DisplayDesc.UseLight = false
	OBJ_21:SetModelState(1)
	OBJ_21.PhysicDesc.Pos.X = 466.5
	OBJ_21.PhysicDesc.Pos.Y = 17.9688
	OBJ_21.PhysicDesc.Pos.Z = 517.5
	OBJ_21.PhysicDesc.Pos.Rotation = 270
	OBJ_21:SetPhysicalActorType(1)
	OBJ_21:SetPhysicalShape(5)
	OBJ_21.PhysicDesc.Collidable = true
	OBJ_21.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_9.phy"
	OBJ_21H = ActorHandler(OBJ_21)
	environment:AddActorObject(OBJ_21H)


	OBJ_22 = ActorObjectInfo(22)
	OBJ_22:SetRenderType(1)
	OBJ_22.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_10.osgb"
	OBJ_22.DisplayDesc.UseLight = false
	OBJ_22:SetModelState(1)
	OBJ_22.PhysicDesc.Pos.X = 444.75
	OBJ_22.PhysicDesc.Pos.Y = 11.7188
	OBJ_22.PhysicDesc.Pos.Z = 520.002
	OBJ_22.PhysicDesc.Pos.Rotation = 360
	OBJ_22:SetPhysicalActorType(1)
	OBJ_22:SetPhysicalShape(5)
	OBJ_22.PhysicDesc.Collidable = true
	OBJ_22.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_10.phy"
	OBJ_22H = ActorHandler(OBJ_22)
	environment:AddActorObject(OBJ_22H)


	OBJ_23 = ActorObjectInfo(23)
	OBJ_23:SetRenderType(1)
	OBJ_23.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_3.osgb"
	OBJ_23.DisplayDesc.UseLight = false
	OBJ_23:SetModelState(1)
	OBJ_23.PhysicDesc.Pos.X = 505.5
	OBJ_23.PhysicDesc.Pos.Y = 17.9688
	OBJ_23.PhysicDesc.Pos.Z = 518.5
	OBJ_23.PhysicDesc.Pos.Rotation = 270
	OBJ_23:SetPhysicalActorType(1)
	OBJ_23:SetPhysicalShape(5)
	OBJ_23.PhysicDesc.Collidable = true
	OBJ_23.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_3.phy"
	OBJ_23H = ActorHandler(OBJ_23)
	environment:AddActorObject(OBJ_23H)


	OBJ_24 = ActorObjectInfo(24)
	OBJ_24:SetRenderType(1)
	OBJ_24.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_2.osgb"
	OBJ_24.DisplayDesc.UseLight = false
	OBJ_24:SetModelState(1)
	OBJ_24.PhysicDesc.Pos.X = 512.5
	OBJ_24.PhysicDesc.Pos.Y = 11.5195
	OBJ_24.PhysicDesc.Pos.Z = 486.5
	OBJ_24.PhysicDesc.Pos.Rotation = 270
	OBJ_24:SetPhysicalActorType(1)
	OBJ_24:SetPhysicalShape(5)
	OBJ_24.PhysicDesc.Collidable = true
	OBJ_24.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_2.phy"
	OBJ_24H = ActorHandler(OBJ_24)
	environment:AddActorObject(OBJ_24H)


	OBJ_25 = ActorObjectInfo(25)
	OBJ_25:SetRenderType(1)
	OBJ_25.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_4.osgb"
	OBJ_25.DisplayDesc.UseLight = false
	OBJ_25:SetModelState(1)
	OBJ_25.PhysicDesc.Pos.X = 517.5
	OBJ_25.PhysicDesc.Pos.Y = 14.6484
	OBJ_25.PhysicDesc.Pos.Z = 514.5
	OBJ_25.PhysicDesc.Pos.Rotation = 180
	OBJ_25:SetPhysicalActorType(1)
	OBJ_25:SetPhysicalShape(5)
	OBJ_25.PhysicDesc.Collidable = true
	OBJ_25.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_4.phy"
	OBJ_25H = ActorHandler(OBJ_25)
	environment:AddActorObject(OBJ_25H)


	OBJ_26 = ActorObjectInfo(26)
	OBJ_26:SetRenderType(1)
	OBJ_26.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_5.osgb"
	OBJ_26.DisplayDesc.UseLight = false
	OBJ_26:SetModelState(1)
	OBJ_26.PhysicDesc.Pos.X = 517.5
	OBJ_26.PhysicDesc.Pos.Y = 14.6484
	OBJ_26.PhysicDesc.Pos.Z = 514.5
	OBJ_26.PhysicDesc.Pos.Rotation = 180
	OBJ_26:SetPhysicalActorType(1)
	OBJ_26:SetPhysicalShape(5)
	OBJ_26.PhysicDesc.Collidable = true
	OBJ_26.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_5.phy"
	OBJ_26H = ActorHandler(OBJ_26)
	environment:AddActorObject(OBJ_26H)


	OBJ_27 = ActorObjectInfo(27)
	OBJ_27:SetRenderType(1)
	OBJ_27.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_6.osgb"
	OBJ_27.DisplayDesc.UseLight = false
	OBJ_27:SetModelState(1)
	OBJ_27.PhysicDesc.Pos.X = 517.5
	OBJ_27.PhysicDesc.Pos.Y = 14.6484
	OBJ_27.PhysicDesc.Pos.Z = 514.5
	OBJ_27.PhysicDesc.Pos.Rotation = 180
	OBJ_27:SetPhysicalActorType(1)
	OBJ_27:SetPhysicalShape(5)
	OBJ_27.PhysicDesc.Collidable = true
	OBJ_27.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_6.phy"
	OBJ_27H = ActorHandler(OBJ_27)
	environment:AddActorObject(OBJ_27H)


	OBJ_28 = ActorObjectInfo(28)
	OBJ_28:SetRenderType(1)
	OBJ_28.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_7.osgb"
	OBJ_28.DisplayDesc.UseLight = false
	OBJ_28:SetModelState(1)
	OBJ_28.PhysicDesc.Pos.X = 517.5
	OBJ_28.PhysicDesc.Pos.Y = 14.6484
	OBJ_28.PhysicDesc.Pos.Z = 514.5
	OBJ_28.PhysicDesc.Pos.Rotation = 180
	OBJ_28:SetPhysicalActorType(1)
	OBJ_28:SetPhysicalShape(5)
	OBJ_28.PhysicDesc.Collidable = true
	OBJ_28.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_7.phy"
	OBJ_28H = ActorHandler(OBJ_28)
	environment:AddActorObject(OBJ_28H)


	OBJ_29 = ActorObjectInfo(29)
	OBJ_29:SetRenderType(1)
	OBJ_29.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_8.osgb"
	OBJ_29.DisplayDesc.UseLight = false
	OBJ_29:SetModelState(1)
	OBJ_29.PhysicDesc.Pos.X = 517.5
	OBJ_29.PhysicDesc.Pos.Y = 14.6484
	OBJ_29.PhysicDesc.Pos.Z = 514.5
	OBJ_29.PhysicDesc.Pos.Rotation = 180
	OBJ_29:SetPhysicalActorType(1)
	OBJ_29:SetPhysicalShape(5)
	OBJ_29.PhysicDesc.Collidable = true
	OBJ_29.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_8.phy"
	OBJ_29H = ActorHandler(OBJ_29)
	environment:AddActorObject(OBJ_29H)


	OBJ_30 = ActorObjectInfo(30)
	OBJ_30:SetRenderType(1)
	OBJ_30.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_9.osgb"
	OBJ_30.DisplayDesc.UseLight = false
	OBJ_30:SetModelState(1)
	OBJ_30.PhysicDesc.Pos.X = 517.5
	OBJ_30.PhysicDesc.Pos.Y = 14.6484
	OBJ_30.PhysicDesc.Pos.Z = 514.5
	OBJ_30.PhysicDesc.Pos.Rotation = 180
	OBJ_30:SetPhysicalActorType(1)
	OBJ_30:SetPhysicalShape(5)
	OBJ_30.PhysicDesc.Collidable = true
	OBJ_30.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_9.phy"
	OBJ_30H = ActorHandler(OBJ_30)
	environment:AddActorObject(OBJ_30H)


	OBJ_31 = ActorObjectInfo(31)
	OBJ_31:SetRenderType(1)
	OBJ_31.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_4.osgb"
	OBJ_31.DisplayDesc.UseLight = false
	OBJ_31:SetModelState(1)
	OBJ_31.PhysicDesc.Pos.X = 517.5
	OBJ_31.PhysicDesc.Pos.Y = 14.6484
	OBJ_31.PhysicDesc.Pos.Z = 508.5
	OBJ_31.PhysicDesc.Pos.Rotation = 180
	OBJ_31:SetPhysicalActorType(1)
	OBJ_31:SetPhysicalShape(5)
	OBJ_31.PhysicDesc.Collidable = true
	OBJ_31.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_4.phy"
	OBJ_31H = ActorHandler(OBJ_31)
	environment:AddActorObject(OBJ_31H)


	OBJ_32 = ActorObjectInfo(32)
	OBJ_32:SetRenderType(1)
	OBJ_32.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_5.osgb"
	OBJ_32.DisplayDesc.UseLight = false
	OBJ_32:SetModelState(1)
	OBJ_32.PhysicDesc.Pos.X = 517.5
	OBJ_32.PhysicDesc.Pos.Y = 14.6484
	OBJ_32.PhysicDesc.Pos.Z = 508.5
	OBJ_32.PhysicDesc.Pos.Rotation = 180
	OBJ_32:SetPhysicalActorType(1)
	OBJ_32:SetPhysicalShape(5)
	OBJ_32.PhysicDesc.Collidable = true
	OBJ_32.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_5.phy"
	OBJ_32H = ActorHandler(OBJ_32)
	environment:AddActorObject(OBJ_32H)


	OBJ_33 = ActorObjectInfo(33)
	OBJ_33:SetRenderType(1)
	OBJ_33.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_6.osgb"
	OBJ_33.DisplayDesc.UseLight = false
	OBJ_33:SetModelState(1)
	OBJ_33.PhysicDesc.Pos.X = 517.5
	OBJ_33.PhysicDesc.Pos.Y = 14.6484
	OBJ_33.PhysicDesc.Pos.Z = 508.5
	OBJ_33.PhysicDesc.Pos.Rotation = 180
	OBJ_33:SetPhysicalActorType(1)
	OBJ_33:SetPhysicalShape(5)
	OBJ_33.PhysicDesc.Collidable = true
	OBJ_33.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_6.phy"
	OBJ_33H = ActorHandler(OBJ_33)
	environment:AddActorObject(OBJ_33H)


	OBJ_34 = ActorObjectInfo(34)
	OBJ_34:SetRenderType(1)
	OBJ_34.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_7.osgb"
	OBJ_34.DisplayDesc.UseLight = false
	OBJ_34:SetModelState(1)
	OBJ_34.PhysicDesc.Pos.X = 517.5
	OBJ_34.PhysicDesc.Pos.Y = 14.6484
	OBJ_34.PhysicDesc.Pos.Z = 508.5
	OBJ_34.PhysicDesc.Pos.Rotation = 180
	OBJ_34:SetPhysicalActorType(1)
	OBJ_34:SetPhysicalShape(5)
	OBJ_34.PhysicDesc.Collidable = true
	OBJ_34.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_7.phy"
	OBJ_34H = ActorHandler(OBJ_34)
	environment:AddActorObject(OBJ_34H)


	OBJ_35 = ActorObjectInfo(35)
	OBJ_35:SetRenderType(1)
	OBJ_35.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_8.osgb"
	OBJ_35.DisplayDesc.UseLight = false
	OBJ_35:SetModelState(1)
	OBJ_35.PhysicDesc.Pos.X = 517.5
	OBJ_35.PhysicDesc.Pos.Y = 14.6484
	OBJ_35.PhysicDesc.Pos.Z = 508.5
	OBJ_35.PhysicDesc.Pos.Rotation = 180
	OBJ_35:SetPhysicalActorType(1)
	OBJ_35:SetPhysicalShape(5)
	OBJ_35.PhysicDesc.Collidable = true
	OBJ_35.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_8.phy"
	OBJ_35H = ActorHandler(OBJ_35)
	environment:AddActorObject(OBJ_35H)


	OBJ_36 = ActorObjectInfo(36)
	OBJ_36:SetRenderType(1)
	OBJ_36.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_9.osgb"
	OBJ_36.DisplayDesc.UseLight = false
	OBJ_36:SetModelState(1)
	OBJ_36.PhysicDesc.Pos.X = 517.5
	OBJ_36.PhysicDesc.Pos.Y = 14.6484
	OBJ_36.PhysicDesc.Pos.Z = 508.5
	OBJ_36.PhysicDesc.Pos.Rotation = 180
	OBJ_36:SetPhysicalActorType(1)
	OBJ_36:SetPhysicalShape(5)
	OBJ_36.PhysicDesc.Collidable = true
	OBJ_36.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_9.phy"
	OBJ_36H = ActorHandler(OBJ_36)
	environment:AddActorObject(OBJ_36H)


	OBJ_37 = ActorObjectInfo(37)
	OBJ_37:SetRenderType(1)
	OBJ_37.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_4.osgb"
	OBJ_37.DisplayDesc.UseLight = false
	OBJ_37:SetModelState(1)
	OBJ_37.PhysicDesc.Pos.X = 502.5
	OBJ_37.PhysicDesc.Pos.Y = 17.9688
	OBJ_37.PhysicDesc.Pos.Z = 512.5
	OBJ_37.PhysicDesc.Pos.Rotation = 360
	OBJ_37:SetPhysicalActorType(1)
	OBJ_37:SetPhysicalShape(5)
	OBJ_37.PhysicDesc.Collidable = true
	OBJ_37.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_4.phy"
	OBJ_37H = ActorHandler(OBJ_37)
	environment:AddActorObject(OBJ_37H)


	OBJ_38 = ActorObjectInfo(38)
	OBJ_38:SetRenderType(1)
	OBJ_38.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_5.osgb"
	OBJ_38.DisplayDesc.UseLight = false
	OBJ_38:SetModelState(1)
	OBJ_38.PhysicDesc.Pos.X = 502.5
	OBJ_38.PhysicDesc.Pos.Y = 17.9688
	OBJ_38.PhysicDesc.Pos.Z = 512.5
	OBJ_38.PhysicDesc.Pos.Rotation = 360
	OBJ_38:SetPhysicalActorType(1)
	OBJ_38:SetPhysicalShape(5)
	OBJ_38.PhysicDesc.Collidable = true
	OBJ_38.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_5.phy"
	OBJ_38H = ActorHandler(OBJ_38)
	environment:AddActorObject(OBJ_38H)


	OBJ_39 = ActorObjectInfo(39)
	OBJ_39:SetRenderType(1)
	OBJ_39.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_6.osgb"
	OBJ_39.DisplayDesc.UseLight = false
	OBJ_39:SetModelState(1)
	OBJ_39.PhysicDesc.Pos.X = 502.5
	OBJ_39.PhysicDesc.Pos.Y = 17.9688
	OBJ_39.PhysicDesc.Pos.Z = 512.5
	OBJ_39.PhysicDesc.Pos.Rotation = 360
	OBJ_39:SetPhysicalActorType(1)
	OBJ_39:SetPhysicalShape(5)
	OBJ_39.PhysicDesc.Collidable = true
	OBJ_39.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_6.phy"
	OBJ_39H = ActorHandler(OBJ_39)
	environment:AddActorObject(OBJ_39H)


	OBJ_40 = ActorObjectInfo(40)
	OBJ_40:SetRenderType(1)
	OBJ_40.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_7.osgb"
	OBJ_40.DisplayDesc.UseLight = false
	OBJ_40:SetModelState(1)
	OBJ_40.PhysicDesc.Pos.X = 502.5
	OBJ_40.PhysicDesc.Pos.Y = 17.9688
	OBJ_40.PhysicDesc.Pos.Z = 512.5
	OBJ_40.PhysicDesc.Pos.Rotation = 360
	OBJ_40:SetPhysicalActorType(1)
	OBJ_40:SetPhysicalShape(5)
	OBJ_40.PhysicDesc.Collidable = true
	OBJ_40.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_7.phy"
	OBJ_40H = ActorHandler(OBJ_40)
	environment:AddActorObject(OBJ_40H)


	OBJ_41 = ActorObjectInfo(41)
	OBJ_41:SetRenderType(1)
	OBJ_41.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_8.osgb"
	OBJ_41.DisplayDesc.UseLight = false
	OBJ_41:SetModelState(1)
	OBJ_41.PhysicDesc.Pos.X = 502.5
	OBJ_41.PhysicDesc.Pos.Y = 17.9688
	OBJ_41.PhysicDesc.Pos.Z = 512.5
	OBJ_41.PhysicDesc.Pos.Rotation = 360
	OBJ_41:SetPhysicalActorType(1)
	OBJ_41:SetPhysicalShape(5)
	OBJ_41.PhysicDesc.Collidable = true
	OBJ_41.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_8.phy"
	OBJ_41H = ActorHandler(OBJ_41)
	environment:AddActorObject(OBJ_41H)


	OBJ_42 = ActorObjectInfo(42)
	OBJ_42:SetRenderType(1)
	OBJ_42.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_9.osgb"
	OBJ_42.DisplayDesc.UseLight = false
	OBJ_42:SetModelState(1)
	OBJ_42.PhysicDesc.Pos.X = 502.5
	OBJ_42.PhysicDesc.Pos.Y = 17.9688
	OBJ_42.PhysicDesc.Pos.Z = 512.5
	OBJ_42.PhysicDesc.Pos.Rotation = 360
	OBJ_42:SetPhysicalActorType(1)
	OBJ_42:SetPhysicalShape(5)
	OBJ_42.PhysicDesc.Collidable = true
	OBJ_42.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_9.phy"
	OBJ_42H = ActorHandler(OBJ_42)
	environment:AddActorObject(OBJ_42H)


	OBJ_43 = ActorObjectInfo(43)
	OBJ_43:SetRenderType(1)
	OBJ_43.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_12.osgb"
	OBJ_43.DisplayDesc.UseLight = false
	OBJ_43:SetModelState(1)
	OBJ_43.PhysicDesc.Pos.X = 530.5
	OBJ_43.PhysicDesc.Pos.Y = 0
	OBJ_43.PhysicDesc.Pos.Z = 492.5
	OBJ_43.PhysicDesc.Pos.Rotation = 270
	OBJ_43:SetPhysicalActorType(1)
	OBJ_43:SetPhysicalShape(5)
	OBJ_43.PhysicDesc.Collidable = true
	OBJ_43.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_12.phy"
	OBJ_43H = ActorHandler(OBJ_43)
	environment:AddActorObject(OBJ_43H)


	OBJ_44 = ActorObjectInfo(44)
	OBJ_44:SetRenderType(1)
	OBJ_44.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_12.osgb"
	OBJ_44.DisplayDesc.UseLight = false
	OBJ_44:SetModelState(1)
	OBJ_44.PhysicDesc.Pos.X = 533.5
	OBJ_44.PhysicDesc.Pos.Y = 0
	OBJ_44.PhysicDesc.Pos.Z = 490.5
	OBJ_44.PhysicDesc.Pos.Rotation = 270
	OBJ_44:SetPhysicalActorType(1)
	OBJ_44:SetPhysicalShape(5)
	OBJ_44.PhysicDesc.Collidable = true
	OBJ_44.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_12.phy"
	OBJ_44H = ActorHandler(OBJ_44)
	environment:AddActorObject(OBJ_44H)


	OBJ_45 = ActorObjectInfo(45)
	OBJ_45:SetRenderType(1)
	OBJ_45.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_12.osgb"
	OBJ_45.DisplayDesc.UseLight = false
	OBJ_45:SetModelState(1)
	OBJ_45.PhysicDesc.Pos.X = 534.5
	OBJ_45.PhysicDesc.Pos.Y = 0
	OBJ_45.PhysicDesc.Pos.Z = 493.5
	OBJ_45.PhysicDesc.Pos.Rotation = 270
	OBJ_45:SetPhysicalActorType(1)
	OBJ_45:SetPhysicalShape(5)
	OBJ_45.PhysicDesc.Collidable = true
	OBJ_45.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_12.phy"
	OBJ_45H = ActorHandler(OBJ_45)
	environment:AddActorObject(OBJ_45H)


	OBJ_46 = ActorObjectInfo(46)
	OBJ_46:SetRenderType(1)
	OBJ_46.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_13.osgb"
	OBJ_46.DisplayDesc.UseLight = false
	OBJ_46:SetModelState(1)
	OBJ_46.PhysicDesc.Pos.X = 531.5
	OBJ_46.PhysicDesc.Pos.Y = 0
	OBJ_46.PhysicDesc.Pos.Z = 490.5
	OBJ_46.PhysicDesc.Pos.Rotation = 270
	OBJ_46:SetPhysicalActorType(1)
	OBJ_46:SetPhysicalShape(5)
	OBJ_46.PhysicDesc.Collidable = true
	OBJ_46.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_13.phy"
	OBJ_46H = ActorHandler(OBJ_46)
	environment:AddActorObject(OBJ_46H)


	OBJ_47 = ActorObjectInfo(47)
	OBJ_47:SetRenderType(1)
	OBJ_47.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_14.osgb"
	OBJ_47.DisplayDesc.UseLight = false
	OBJ_47:SetModelState(1)
	OBJ_47.PhysicDesc.Pos.X = 533.5
	OBJ_47.PhysicDesc.Pos.Y = 0
	OBJ_47.PhysicDesc.Pos.Z = 492.5
	OBJ_47.PhysicDesc.Pos.Rotation = 270
	OBJ_47:SetPhysicalActorType(1)
	OBJ_47:SetPhysicalShape(5)
	OBJ_47.PhysicDesc.Collidable = true
	OBJ_47.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_14.phy"
	OBJ_47H = ActorHandler(OBJ_47)
	environment:AddActorObject(OBJ_47H)


	OBJ_48 = ActorObjectInfo(48)
	OBJ_48:SetRenderType(1)
	OBJ_48.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_15.osgb"
	OBJ_48.DisplayDesc.UseLight = false
	OBJ_48:SetModelState(1)
	OBJ_48.PhysicDesc.Pos.X = 540
	OBJ_48.PhysicDesc.Pos.Y = 0
	OBJ_48.PhysicDesc.Pos.Z = 500
	OBJ_48.PhysicDesc.Pos.Rotation = 90
	OBJ_48:SetPhysicalActorType(1)
	OBJ_48:SetPhysicalShape(5)
	OBJ_48.PhysicDesc.Collidable = true
	OBJ_48.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_15.phy"
	OBJ_48H = ActorHandler(OBJ_48)
	environment:AddActorObject(OBJ_48H)


	OBJ_49 = ActorObjectInfo(49)
	OBJ_49:SetRenderType(1)
	OBJ_49.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_16.osgb"
	OBJ_49.DisplayDesc.UseLight = false
	OBJ_49:SetModelState(1)
	OBJ_49.PhysicDesc.Pos.X = 536
	OBJ_49.PhysicDesc.Pos.Y = 4.6875
	OBJ_49.PhysicDesc.Pos.Z = 498
	OBJ_49.PhysicDesc.Pos.Rotation = 90
	OBJ_49:SetPhysicalActorType(1)
	OBJ_49:SetPhysicalShape(5)
	OBJ_49.PhysicDesc.Collidable = true
	OBJ_49.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_16.phy"
	OBJ_49H = ActorHandler(OBJ_49)
	environment:AddActorObject(OBJ_49H)


	OBJ_50 = ActorObjectInfo(50)
	OBJ_50:SetRenderType(1)
	OBJ_50.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_17.osgb"
	OBJ_50.DisplayDesc.UseLight = false
	OBJ_50:SetModelState(1)
	OBJ_50.PhysicDesc.Pos.X = 530
	OBJ_50.PhysicDesc.Pos.Y = 1.36719
	OBJ_50.PhysicDesc.Pos.Z = 499
	OBJ_50.PhysicDesc.Pos.Rotation = 90
	OBJ_50:SetPhysicalActorType(1)
	OBJ_50:SetPhysicalShape(5)
	OBJ_50.PhysicDesc.Collidable = true
	OBJ_50.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_17.phy"
	OBJ_50H = ActorHandler(OBJ_50)
	environment:AddActorObject(OBJ_50H)


	OBJ_51 = ActorObjectInfo(51)
	OBJ_51:SetRenderType(1)
	OBJ_51.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_12.osgb"
	OBJ_51.DisplayDesc.UseLight = false
	OBJ_51:SetModelState(1)
	OBJ_51.PhysicDesc.Pos.X = 488.5
	OBJ_51.PhysicDesc.Pos.Y = 2.73438
	OBJ_51.PhysicDesc.Pos.Z = 496.5
	OBJ_51.PhysicDesc.Pos.Rotation = 270
	OBJ_51:SetPhysicalActorType(1)
	OBJ_51:SetPhysicalShape(5)
	OBJ_51.PhysicDesc.Collidable = true
	OBJ_51.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_12.phy"
	OBJ_51H = ActorHandler(OBJ_51)
	environment:AddActorObject(OBJ_51H)


	OBJ_52 = ActorObjectInfo(52)
	OBJ_52:SetRenderType(1)
	OBJ_52.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_12.osgb"
	OBJ_52.DisplayDesc.UseLight = false
	OBJ_52:SetModelState(1)
	OBJ_52.PhysicDesc.Pos.X = 484.5
	OBJ_52.PhysicDesc.Pos.Y = 2.73438
	OBJ_52.PhysicDesc.Pos.Z = 499.5
	OBJ_52.PhysicDesc.Pos.Rotation = 270
	OBJ_52:SetPhysicalActorType(1)
	OBJ_52:SetPhysicalShape(5)
	OBJ_52.PhysicDesc.Collidable = true
	OBJ_52.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_12.phy"
	OBJ_52H = ActorHandler(OBJ_52)
	environment:AddActorObject(OBJ_52H)


	OBJ_53 = ActorObjectInfo(53)
	OBJ_53:SetRenderType(1)
	OBJ_53.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_3.osgb"
	OBJ_53.DisplayDesc.UseLight = false
	OBJ_53:SetModelState(1)
	OBJ_53.PhysicDesc.Pos.X = 482.375
	OBJ_53.PhysicDesc.Pos.Y = 17.9688
	OBJ_53.PhysicDesc.Pos.Z = 520
	OBJ_53.PhysicDesc.Pos.Rotation = 270
	OBJ_53:SetPhysicalActorType(1)
	OBJ_53:SetPhysicalShape(5)
	OBJ_53.PhysicDesc.Collidable = true
	OBJ_53.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_3.phy"
	OBJ_53H = ActorHandler(OBJ_53)
	environment:AddActorObject(OBJ_53H)


	OBJ_54 = ActorObjectInfo(54)
	OBJ_54:SetRenderType(1)
	OBJ_54.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_2.osgb"
	OBJ_54.DisplayDesc.UseLight = false
	OBJ_54:SetModelState(1)
	OBJ_54.PhysicDesc.Pos.X = 481.125
	OBJ_54.PhysicDesc.Pos.Y = 17.9688
	OBJ_54.PhysicDesc.Pos.Z = 517.125
	OBJ_54.PhysicDesc.Pos.Rotation = 270
	OBJ_54:SetPhysicalActorType(1)
	OBJ_54:SetPhysicalShape(5)
	OBJ_54.PhysicDesc.Collidable = true
	OBJ_54.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_2.phy"
	OBJ_54H = ActorHandler(OBJ_54)
	environment:AddActorObject(OBJ_54H)


	OBJ_55 = ActorObjectInfo(55)
	OBJ_55:SetRenderType(1)
	OBJ_55.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_18.osgb"
	OBJ_55.DisplayDesc.UseLight = false
	OBJ_55:SetModelState(1)
	OBJ_55.PhysicDesc.Pos.X = 532.5
	OBJ_55.PhysicDesc.Pos.Y = 8.39453
	OBJ_55.PhysicDesc.Pos.Z = 522.5
	OBJ_55.PhysicDesc.Pos.Rotation = 270
	OBJ_55:SetPhysicalActorType(1)
	OBJ_55:SetPhysicalShape(5)
	OBJ_55.PhysicDesc.Collidable = true
	OBJ_55.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_18.phy"
	OBJ_55H = ActorHandler(OBJ_55)
	environment:AddActorObject(OBJ_55H)


	OBJ_56 = ActorObjectInfo(56)
	OBJ_56:SetRenderType(1)
	OBJ_56.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_19.osgb"
	OBJ_56.DisplayDesc.UseLight = false
	OBJ_56:SetModelState(1)
	OBJ_56.PhysicDesc.Pos.X = 532.5
	OBJ_56.PhysicDesc.Pos.Y = 8.39844
	OBJ_56.PhysicDesc.Pos.Z = 522.5
	OBJ_56.PhysicDesc.Pos.Rotation = 270
	OBJ_56:SetPhysicalActorType(1)
	OBJ_56:SetPhysicalShape(5)
	OBJ_56.PhysicDesc.Collidable = true
	OBJ_56.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_19.phy"
	OBJ_56H = ActorHandler(OBJ_56)
	environment:AddActorObject(OBJ_56H)


	OBJ_57 = ActorObjectInfo(57)
	OBJ_57:SetRenderType(1)
	OBJ_57.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_20.osgb"
	OBJ_57.DisplayDesc.UseLight = false
	OBJ_57:SetModelState(1)
	OBJ_57.PhysicDesc.Pos.X = 532.75
	OBJ_57.PhysicDesc.Pos.Y = 8.35547
	OBJ_57.PhysicDesc.Pos.Z = 522.75
	OBJ_57.PhysicDesc.Pos.Rotation = 270
	OBJ_57:SetPhysicalActorType(1)
	OBJ_57:SetPhysicalShape(5)
	OBJ_57.PhysicDesc.Collidable = true
	OBJ_57.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_20.phy"
	OBJ_57H = ActorHandler(OBJ_57)
	environment:AddActorObject(OBJ_57H)


	OBJ_58 = ActorObjectInfo(58)
	OBJ_58:SetRenderType(1)
	OBJ_58.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_20.osgb"
	OBJ_58.DisplayDesc.UseLight = false
	OBJ_58:SetModelState(1)
	OBJ_58.PhysicDesc.Pos.X = 532.75
	OBJ_58.PhysicDesc.Pos.Y = 8.35547
	OBJ_58.PhysicDesc.Pos.Z = 522.25
	OBJ_58.PhysicDesc.Pos.Rotation = 360
	OBJ_58:SetPhysicalActorType(1)
	OBJ_58:SetPhysicalShape(5)
	OBJ_58.PhysicDesc.Collidable = true
	OBJ_58.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_20.phy"
	OBJ_58H = ActorHandler(OBJ_58)
	environment:AddActorObject(OBJ_58H)


	OBJ_59 = ActorObjectInfo(59)
	OBJ_59:SetRenderType(1)
	OBJ_59.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_20.osgb"
	OBJ_59.DisplayDesc.UseLight = false
	OBJ_59:SetModelState(1)
	OBJ_59.PhysicDesc.Pos.X = 532.25
	OBJ_59.PhysicDesc.Pos.Y = 8.35547
	OBJ_59.PhysicDesc.Pos.Z = 522.25
	OBJ_59.PhysicDesc.Pos.Rotation = 90
	OBJ_59:SetPhysicalActorType(1)
	OBJ_59:SetPhysicalShape(5)
	OBJ_59.PhysicDesc.Collidable = true
	OBJ_59.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_20.phy"
	OBJ_59H = ActorHandler(OBJ_59)
	environment:AddActorObject(OBJ_59H)


	OBJ_60 = ActorObjectInfo(60)
	OBJ_60:SetRenderType(1)
	OBJ_60.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_20.osgb"
	OBJ_60.DisplayDesc.UseLight = false
	OBJ_60:SetModelState(1)
	OBJ_60.PhysicDesc.Pos.X = 532.25
	OBJ_60.PhysicDesc.Pos.Y = 8.35547
	OBJ_60.PhysicDesc.Pos.Z = 522.75
	OBJ_60.PhysicDesc.Pos.Rotation = 180
	OBJ_60:SetPhysicalActorType(1)
	OBJ_60:SetPhysicalShape(5)
	OBJ_60.PhysicDesc.Collidable = true
	OBJ_60.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_20.phy"
	OBJ_60H = ActorHandler(OBJ_60)
	environment:AddActorObject(OBJ_60H)


	OBJ_61 = ActorObjectInfo(61)
	OBJ_61:SetRenderType(1)
	OBJ_61.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_21.osgb"
	OBJ_61.DisplayDesc.UseLight = false
	OBJ_61:SetModelState(1)
	OBJ_61.PhysicDesc.Pos.X = 529.375
	OBJ_61.PhysicDesc.Pos.Y = 7.22266
	OBJ_61.PhysicDesc.Pos.Z = 511.625
	OBJ_61.PhysicDesc.Pos.Rotation = 360
	OBJ_61:SetPhysicalActorType(1)
	OBJ_61:SetPhysicalShape(5)
	OBJ_61.PhysicDesc.Collidable = true
	OBJ_61.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_21.phy"
	OBJ_61H = ActorHandler(OBJ_61)
	environment:AddActorObject(OBJ_61H)


	OBJ_62 = ActorObjectInfo(62)
	OBJ_62:SetRenderType(1)
	OBJ_62.DisplayDesc.ModelName = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_10.osgb"
	OBJ_62.DisplayDesc.UseLight = false
	OBJ_62:SetModelState(1)
	OBJ_62.PhysicDesc.Pos.X = 521
	OBJ_62.PhysicDesc.Pos.Y = 1.95313
	OBJ_62.PhysicDesc.Pos.Z = 500.75
	OBJ_62.PhysicDesc.Pos.Rotation = 90
	OBJ_62:SetPhysicalActorType(1)
	OBJ_62:SetPhysicalShape(5)
	OBJ_62.PhysicDesc.Collidable = true
	OBJ_62.PhysicDesc.Filename = "Worlds/Lba2Original/Islands/SOUSCELB_OBJ_10.phy"
	OBJ_62H = ActorHandler(OBJ_62)
	environment:AddActorObject(OBJ_62H)
end
