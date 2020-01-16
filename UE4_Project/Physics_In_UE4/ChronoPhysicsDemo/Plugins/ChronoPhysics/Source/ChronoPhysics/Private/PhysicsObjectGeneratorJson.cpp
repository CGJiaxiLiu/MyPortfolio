// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsObjectGeneratorJson.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void APhysicsObjectGeneratorJson::PhysicsObjectConstruct()
{
	auto Jsonlocation = FPaths::Combine(FPaths::ProjectDir(), FString("Config"), JsonFileName + FString(".json"));
	FString JsonData;

	TMap<FString, AChBody_GeneratedActor*> bodyMap;
	TArray<AChLinkActor*> linkArray;

	if (FFileHelper::LoadFileToString(JsonData, *Jsonlocation)) {
		//UE_LOG(LogTemp, Warning, TEXT("Data:%s"), *JsonData);

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonData);

		if (FJsonSerializer::Deserialize(Reader, JsonObject)) {
			UE_LOG(LogTemp, Warning, TEXT("Deserialize Success"));

			auto bodyJsonArray = JsonObject->GetArrayField("Body");
			UE_LOG(LogTemp, Warning, TEXT("Body Num:%d"), bodyJsonArray.Num());
			auto linkJsonArray = JsonObject->GetArrayField("Link");
			UE_LOG(LogTemp, Warning, TEXT("Link Num:%d"), linkJsonArray.Num());

			for (auto bodyItem : bodyJsonArray) {

				AChBody_GeneratedActor* body;
				if (bodyItem->AsObject()->GetStringField("Shape").ToLower().Compare(FString("box")) == 0) {
					body = NewChBodyActor(EShapeType::Box);
				}
				else if(bodyItem->AsObject()->GetStringField("Shape").ToLower().Compare(FString("cylinder")) == 0) {
					body = NewChBodyActor(EShapeType::Cylinder);
				}
				else{
					body = NewChBodyActor(EShapeType::Sphere);
				}

				if (bodyItem->AsObject()->HasField("Position")) {
					if (bodyItem->AsObject()->GetArrayField("Position").Num() >= 3) {
						float tx = bodyItem->AsObject()->GetArrayField("Position")[0]->AsNumber();
						float ty = bodyItem->AsObject()->GetArrayField("Position")[1]->AsNumber();
						float tz = bodyItem->AsObject()->GetArrayField("Position")[2]->AsNumber();
						body->SetActorLocation(FVector(tx, ty, tz));
					}
				}
				if (bodyItem->AsObject()->HasField("Rotation")) {
					if (bodyItem->AsObject()->GetArrayField("Rotation").Num() >= 3) {
						float rx = bodyItem->AsObject()->GetArrayField("Rotation")[0]->AsNumber();
						float ry = bodyItem->AsObject()->GetArrayField("Rotation")[1]->AsNumber();
						float rz = bodyItem->AsObject()->GetArrayField("Rotation")[2]->AsNumber();
						body->SetActorRotation(FRotator(rx, ry, rz));
					}
				}
				if (bodyItem->AsObject()->HasField("Scale")) {
					if (bodyItem->AsObject()->GetArrayField("Scale").Num() >= 3) {
						float sx = bodyItem->AsObject()->GetArrayField("Scale")[0]->AsNumber();
						float sy = bodyItem->AsObject()->GetArrayField("Scale")[1]->AsNumber();
						float sz = bodyItem->AsObject()->GetArrayField("Scale")[2]->AsNumber();
						body->SetActorScale3D(FVector(sx, sy, sz));
					}
				}
				if (bodyItem->AsObject()->HasField("Fixed")) {
					if (bodyItem->AsObject()->GetBoolField("Fixed")) {
						body->ChComp->isFixed = true;
					}
				}
				if (bodyItem->AsObject()->HasField("Density")) {
					body->ChComp->Density = bodyItem->AsObject()->GetNumberField("Density");
				}
				body->ChComp->PhysicsObjectConstruct();
				bodyMap.Add(bodyItem->AsObject()->GetStringField("Name"), body);
			}

			for (auto linkItem : linkJsonArray) {

				AChBody_GeneratedActor* target1 = nullptr;
				AChBody_GeneratedActor* target2 = nullptr;

				if (linkItem->AsObject()->HasField("Target")) {
					auto targetArray = linkItem->AsObject()->GetArrayField("Target");
					if (targetArray.Num() >= 2) {
						if (bodyMap.Contains(targetArray[0]->AsString()) && bodyMap.Contains(targetArray[1]->AsString())) {
							target1 = bodyMap[targetArray[0]->AsString()];
							target2 = bodyMap[targetArray[1]->AsString()];
						}
					}
				}

				if (!(target1 && target2)) {
					continue;
				}

				AChLinkActor* link;
				if (linkItem->AsObject()->GetStringField("Type").ToUpper().Compare(FString("REVOLUTE")) == 0) {
					link = NewChLinkActor(ELinkType::REVOLUTE);
				}
				else if (linkItem->AsObject()->GetStringField("Type").ToUpper().Compare(FString("ALIGN")) == 0) {
					link = NewChLinkActor(ELinkType::ALIGN);
				}
				else if (linkItem->AsObject()->GetStringField("Type").ToUpper().Compare(FString("CLEARANCE")) == 0) {
					link = NewChLinkActor(ELinkType::CLEARANCE);
				}
				else if (linkItem->AsObject()->GetStringField("Type").ToUpper().Compare(FString("CYLINDRICAL")) == 0) {
					link = NewChLinkActor(ELinkType::CYLINDRICAL);
				}
				else if (linkItem->AsObject()->GetStringField("Type").ToUpper().Compare(FString("ENGINE_ROTATION")) == 0) {
					link = NewChLinkActor(ELinkType::ENGINE_ROTATION);
				}
				else if (linkItem->AsObject()->GetStringField("Type").ToUpper().Compare(FString("ENGINE_SPEED")) == 0) {
					link = NewChLinkActor(ELinkType::ENGINE_SPEED);
				}
				else if (linkItem->AsObject()->GetStringField("Type").ToUpper().Compare(FString("ENGINE_TORQUE")) == 0) {
					link = NewChLinkActor(ELinkType::ENGINE_TORQUE);
				}
				else if (linkItem->AsObject()->GetStringField("Type").ToUpper().Compare(FString("LOCK")) == 0) {
					link = NewChLinkActor(ELinkType::LOCK);
				}
				else if (linkItem->AsObject()->GetStringField("Type").ToUpper().Compare(FString("OLDHAM")) == 0) {
					link = NewChLinkActor(ELinkType::OLDHAM);
				}
				else if (linkItem->AsObject()->GetStringField("Type").ToUpper().Compare(FString("PARALLEL")) == 0) {
					link = NewChLinkActor(ELinkType::PARALLEL);
				}
				else if (linkItem->AsObject()->GetStringField("Type").ToUpper().Compare(FString("PERPEND")) == 0) {
					link = NewChLinkActor(ELinkType::PERPEND);
				}
				else if (linkItem->AsObject()->GetStringField("Type").ToUpper().Compare(FString("PLANEPLANE")) == 0) {
					link = NewChLinkActor(ELinkType::PLANEPLANE);
				}
				else if (linkItem->AsObject()->GetStringField("Type").ToUpper().Compare(FString("POINTLINE")) == 0) {
					link = NewChLinkActor(ELinkType::POINTLINE);
				}
				else if (linkItem->AsObject()->GetStringField("Type").ToUpper().Compare(FString("PRISMATIC")) == 0) {
					link = NewChLinkActor(ELinkType::PRISMATIC);
				}
				else if (linkItem->AsObject()->GetStringField("Type").ToUpper().Compare(FString("REVOLUTEPRISMATIC")) == 0) {
					link = NewChLinkActor(ELinkType::REVOLUTEPRISMATIC);
				}
				else if (linkItem->AsObject()->GetStringField("Type").ToUpper().Compare(FString("SPHERICAL")) == 0) {
					link = NewChLinkActor(ELinkType::SPHERICAL);
				}
				else if (linkItem->AsObject()->GetStringField("Type").ToUpper().Compare(FString("TRAJECTORY")) == 0) {
					link = NewChLinkActor(ELinkType::TRAJECTORY);
				}
				else {
					link = NewChLinkActor(ELinkType::FREE);
				}

				if (linkItem->AsObject()->HasField("Position")) {
					if (linkItem->AsObject()->GetArrayField("Position").Num() >= 3) {
						float tx = linkItem->AsObject()->GetArrayField("Position")[0]->AsNumber();
						float ty = linkItem->AsObject()->GetArrayField("Position")[1]->AsNumber();
						float tz = linkItem->AsObject()->GetArrayField("Position")[2]->AsNumber();
						link->SetActorLocation(FVector(tx, ty, tz));
					}
				}
				if (linkItem->AsObject()->HasField("Rotation")) {
					if (linkItem->AsObject()->GetArrayField("Rotation").Num() >= 3) {
						float rx = linkItem->AsObject()->GetArrayField("Rotation")[0]->AsNumber();
						float ry = linkItem->AsObject()->GetArrayField("Rotation")[1]->AsNumber();
						float rz = linkItem->AsObject()->GetArrayField("Rotation")[2]->AsNumber();
						link->SetActorRotation(FRotator(rx, ry, rz));
					}
				}
				link->target1 = target1;
				link->target2 = target2;

				link->PhysicsObjectConstruct();
				linkArray.Add(link);
			}

			if (bPositionRelative) {
				for (auto kvp : bodyMap) {
					kvp.Value->SetActorLocation(kvp.Value->GetActorLocation() + GetActorLocation());
				}
				for (auto link : linkArray) {
					link->SetActorLocation(link->GetActorLocation() + GetActorLocation());
				}
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Deserialize Fail"));
		}
	}
}
