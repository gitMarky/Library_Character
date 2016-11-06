/**
 This is the basic functionality for displaying an object with additional/optional parts,
 such as accessories or clothing.
 
 @title Modular Appearance
 @author Marky
 @version 0.1.0
 */
 
// Was sollte das Zeug haben?
// - Slots für die einzelnen Teile
// - man kann die Slots mit einer Fallback-Grafik belegen, die angezeigt wird, wenn man nichts benutzt
// - ein Slot zeigt entweder die aktuelle Grafik an, oder die Fallback-Grafik, falls vorhanden, oder nichts
// - die aktuelle Grafik / Attachment-ID des Meshes sollte in einem Array gespeichert sein
// - evtl. sollte das attachde Objekt im Array enthalten sein
// - wenn ein Objekt attached wird, dann sollte die Mesh-Grafik in diesem Objekt abgefragt werden? Oder das Objekt fragt beim Ziel an, ob es einen Skin für diese Grafik hat. Wahrscheinlich ist letzteres besser. Ansonsten eben die Defaul-Grafik
// - Priorität ist explizit nicht notwendig, es kann einfach nur ein Ding je Slot verwendet werden.
// - wahrscheinlich sollte in dem Array jeweils eine Proplist enthalten sein mit {definition, attachmentID, graphicsName, mesh material usw?}

static const LIB_APPEARANCE_SLOT_MAIN = 0;
static const LIB_APPEARANCE_SLOT_SIDE = 1;

local lib_appearance;

func Construction()
{
	lib_appearance = 
	{
		container = CreateObject(Dummy),
		parts = [],
	};

	_inherited(...);
}

func AddAppearance(int slot, skin_definition)
{
	assertSlot(slot);

	if (AppearanceSlots()[slot])
	{
		return false;
	}
	else if (GetType(skin_definition) == C4V_C4Object)
	{
	}
	else if (GetType(skin_definition) == C4V_PropList)
	{
		// prepare the object
		var skin = skin_definition;
		var mesh_object = CreateMeshObject(skin);
		var parent_bone = "skeleton_body";
		var child_bone = "skeleton_body";

		// save info for local use
		skin.AttachObject = mesh_object;
		skin.AttachNr = AttachMesh(mesh_object, parent_bone, child_bone, skin.MeshTransformation ?? Trans_Identity(), skin.Flags);

		AppearanceSlots()[slot] = skin;
		return true;
	}
	else
	{
		FatalError(Format("You can pass C4V_Object or C4V_Proplist, but you passed %v", GetType(skin_definition)));
	}
}

func RemoveAppearance(int slot)
{
	assertSlot(slot);

	if (!AppearanceSlots()[slot])
	{
		FatalError(Format("Trying to remove a slot that does not exist: %d", slot));
	}

	// save definition and remove it
	var info = AppearanceSlots()[slot];
	AppearanceSlots()[slot] = nil;

	// remove the mesh and move the object to the current position
	DetachMesh(info.AttachNr);
	if (info.AttachObject)
	{
		info.AttachObject->Exit();
		info.AttachObject->SetPosition(this->GetX(), this->GetY());
	}
	
	return info;
}

func SetDefaultAppearance(int slot)
{
	assertSlot(slot);
}

func AppearanceSlots()
{
	return lib_appearance.parts;
}

func assertSlot(int slot)
{
	if (slot < 0) FatalError(Format("Slot number must be > 0, got %d", slot));
}

func CreateMeshObject(proplist skin)
{
	var mesh_object = skin.AttachObject ?? CreateObject(skin.GraphicsDefinition ?? Dummy, 0, 0, NO_OWNER);
	mesh_object->SetGraphics(skin.GraphicsName, skin.GraphicsDefinition);
	mesh_object->Enter(lib_appearance.container);
		
	if (skin.MeshMaterial)
	{
		if (GetType(skin.MeshMaterial) == C4V_String)
		{	
			mesh_object->SetMeshMaterial(skin.MeshMaterial);
		}
		else if (GetType(skin.MeshMaterial) == C4V_Array)
		{
			for (var i = 0; i < GetLength(skin.MeshMaterial); ++i)
			{
				mesh_object->SetMeshMaterial(skin.MeshMaterial[i], i);
			}
		}
		else
		{
			FatalError(Format("Unknown format for mesh material: %v - known: C4V_String, C4V_Array", GetType(skin.MeshMaterial)));
		}
	}
	
	mesh_object->SetColor(GetColor());

	return mesh_object;
}


func SetColor(int color)
{
	ForwardToSlots(Global.SetColor, color);
	return _inherited(color);
}


func SetClrModulation(int color, int overlay_id)
{
	ForwardToSlots(Global.SetClrModulation, color, overlay_id);
	return _inherited(color, overlay_id, ...);
}


func SetObjectBlitMode(int dwNewBlitMod, int overlay_id)
{
	ForwardToSlots(Global.SetObjectBlitMode, dwNewBlitMod, overlay_id);
	return _inherited(dwNewBlitMod, overlay_id, ...);
}


func ForwardToSlots(method, var1, var2, var3, var4, var5, var6, var7, var8, var9)
{
	for (var slot in AppearanceSlots())
	{
		if (slot == nil) continue;
		
		if (slot.AttachObject)
		{
			slot.AttachObject->Call(method, var1, var2, var3, var4, var5, var6, var7, var8, var9);
		}
	}
}
