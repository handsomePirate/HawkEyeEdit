#pragma once

#include <SoftwareCore/Singleton.hpp>
#include <SoftwareCore/UUID.hpp>

#define UUIDProvider (::Core::Singleton<::Core::UUIDSystem>::GetInstance())
