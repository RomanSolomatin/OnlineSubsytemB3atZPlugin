// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved
// Plugin written by Philipp Buerki. Copyright 2017. All Rights reserved..

#include "Tests/TestTimeInterface.h"
#include "OnlineSubsystemB3atZUtils.h"

#if WITH_DEV_AUTOMATION_TESTS

void FTestTimeInterface::Test(UWorld* InWorld)
{
	OnlineTime = Online::GetTimeInterface(InWorld, SubsystemName.Len() ? FName(*SubsystemName, FNAME_Find) : NAME_None);
	if (OnlineTime.IsValid())
	{
		// Create and add delegate for the async call
		OnQueryServerUtcTimeCompleteDelegate       = FOnQueryServerUtcTimeCompleteDelegate::CreateRaw(this, &FTestTimeInterface::OnQueryServerUtcTimeComplete);
		OnQueryServerUtcTimeCompleteDelegateHandle = OnlineTime->AddOnQueryServerUtcTimeCompleteDelegate_Handle(OnQueryServerUtcTimeCompleteDelegate);
		// Kick off the async query for server time
		OnlineTime->QueryServerUtcTime();
	}
	else
	{
		UE_LOG(LogB3atZOnline, Warning,
			TEXT("Failed to get server time interface for %s"), *SubsystemName);

		delete this;
	}
}

void FTestTimeInterface::OnQueryServerUtcTimeComplete(bool bWasSuccessful, const FString& DateTimeStr, const FString& Error)
{
	OnlineTime->ClearOnQueryServerUtcTimeCompleteDelegate_Handle(OnQueryServerUtcTimeCompleteDelegateHandle);

	if (bWasSuccessful)
	{
		UE_LOG(LogB3atZOnline, Log, TEXT("Successful query for server time. Result=[%s]"), *DateTimeStr);
	}
	else
	{
		UE_LOG(LogB3atZOnline, Log, TEXT("Failed to query server time. Error=[%s]"), *Error);
	}

	// done with the test
	delete this;
}

#endif //WITH_DEV_AUTOMATION_TESTS
