#pragma once

namespace burger
{

enum UMEventType {
	eEventTypeUnknown,
	eEventTypeRenderUpdate,
	eEventTypeRenderProgressSample,
	eEventTypeCameraZoom,
	eEventTypeCameraDolly,
};

} // burger
