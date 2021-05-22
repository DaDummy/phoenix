// Copyright © 2021 Luis Michaelis
// Licensed under MIT (https://mit-license.org/).
#include <phoenix/detail/error.hh>
#include <phoenix/world/vob_tree.hh>

#include <fmt/format.h>
#include <unordered_map>

namespace phoenix {
	static std::unordered_map<std::string, vob_type> _vob_type_map = {
			{"zCVob", vob_type::zCVob},
			{"zCVobLevelCompo:zCVob", vob_type::zCVobLevelCompo},
			{"oCItem:zCVob", vob_type::oCItem},
			{"oCMOB:zCVob", vob_type::oCMOB},
			{"oCMobInter:oCMOB:zCVob", vob_type::oCMobInter},
			{"oCMobBed:oCMobInter:oCMOB:zCVob", vob_type::oCMobBed},
			{"oCMobFire:oCMobInter:oCMOB:zCVob", vob_type::oCMobFire},
			{"oCMobLadder:oCMobInter:oCMOB:zCVob", vob_type::oCMobLadder},
			{"oCMobSwitch:oCMobInter:oCMOB:zCVob", vob_type::oCMobSwitch},
			{"oCMobWheel:oCMobInter:oCMOB:zCVob", vob_type::oCMobWheel},
			{"oCMobContainer:oCMobInter:oCMOB:zCVob", vob_type::oCMobContainer},
			{"oCMobDoor:oCMobInter:oCMOB:zCVob", vob_type::oCMobDoor},
			{"zCPFXControler:zCVob", vob_type::zCPFXController},
			{"zCVobAnimate:zCVob", vob_type::zCVobAnimate},
			{"zCVobLensFlare:zCVob", vob_type::zCVobLensFlare},
			{"zCVobLight:zCVob", vob_type::zCVobLight},
			{"zCVobSpot:zCVob", vob_type::zCVobSpot},
			{"zCVobStartpoint:zCVob", vob_type::zCVobStartpoint},
			{"zCVobSound:zCVob", vob_type::zCVobSound},
			{"zCVobSoundDaytime:zCVobSound:zCVob", vob_type::zCVobSoundDaytime},
			{"oCZoneMusic:zCVob", vob_type::oCZoneMusic},
			{"oCZoneMusicDefault:oCZoneMusic:zCVob", vob_type::oCZoneMusicDefault},
			{"zCZoneZFog:zCVob", vob_type::zCZoneZFog},
			{"zCZoneZFogDefault:zCZoneZFog:zCVob", vob_type::zCZoneZFogDefault},
			{"zCZoneVobFarPlane:zCVob", vob_type::zCZoneVobFarPlane},
			{"zCZoneVobFarPlaneDefault:zCZoneVobFarPlane:zCVob", vob_type::zCZoneVobFarPlaneDefault},
			{"zCMessageFilter:zCVob", vob_type::zCMessageFilter},
			{"zCCodeMaster:zCVob", vob_type::zCCodeMaster},
			{"zCTrigger:zCVob", vob_type::zCTrigger},
			{"zCTriggerList:zCTrigger:zCVob", vob_type::zCTriggerList},
			{"oCTriggerScript:zCTrigger:zCVob", vob_type::oCTriggerScript},
			{"zCMover:zCTrigger:zCVob", vob_type::zCMover},
			{"oCTriggerChangeLevel:zCTrigger:zCVob", vob_type::oCTriggerChangeLevel},
			{"zCTriggerWorldStart:zCVob", vob_type::zCTriggerWorldStart},
			{"zCTriggerUntouch:zCVob", vob_type::zCTriggerUntouch},
			{"zCCSCamera:zCVob", vob_type::zCCSCamera},
			{"zCCamTrj_KeyFrame:zCVob", vob_type::zCCamTrj_KeyFrame},
			{"oCTouchDamage:zCTouchDamage:zCVob", vob_type::oCTouchDamage},
			{"zCEarthquake:zCVob", vob_type::zCEarthquake},
			{"zCAICamera", vob_type::zCAICamera},
			{"zCMoverControler:zCVob", vob_type::zCMoverController},
			{"zCVobScreenFX:zCVob", vob_type::zCVobScreenFX},
			{"zCVobStair:zCVob", vob_type::zCVobStair},
			{"oCCSTrigger:zCTrigger:zCVob", vob_type::oCCSTrigger},
	};

	namespace vob {
		void base::parse(vob::base& vob, archive_reader_ref& in, game_version version) {
			auto packed = in->read_int() != 0;

			if (packed) {
				auto raw = in->read_raw_bytes();
				reader bin {{(const char*) raw.data(), raw.size()}};

				vob.bbox[0] = bin.read_vec3();
				vob.bbox[1] = bin.read_vec3();
				vob.position = bin.read_vec3();
				vob.rotation[0] = bin.read_vec3();
				vob.rotation[1] = bin.read_vec3();
				vob.rotation[2] = bin.read_vec3();

				u8 bit0 = bin.read_u8();
				u16 bit1;

				if (version == game_version::gothic_1) {
					bit1 = bin.read_u8();
				} else {
					bit1 = bin.read_u16();
				}

				vob.show_visual = static_cast<bool>((bit0 & 0b00000001) >> 0);
				vob.camera_alignment = (bit0 & 0b00000110) >> 1;
				vob.cd_static = static_cast<bool>((bit0 & 0b00001000) >> 3);
				vob.cd_dynamic = static_cast<bool>((bit0 & 0b00010000) >> 4);
				vob.vob_static = static_cast<bool>((bit0 & 0b00100000) >> 5);
				vob.dynamic_shadows = (bit0 & 0b11000000) >> 6;

				bool has_preset_name = static_cast<bool>((bit1 & 0b000000000000001u) >> 0u);
				bool has_vob_name = static_cast<bool>((bit1 & 0b000000000000010u) >> 1u);
				bool has_visual_name = static_cast<bool>((bit1 & 0b000000000000100u) >> 2u);
				bool has_visual_object = static_cast<bool>((bit1 & 0b000000000001000u) >> 3u);
				bool has_ai_object = static_cast<bool>((bit1 & 0b000000000010000u) >> 4u);
				// bool has_event_man_object = static_cast<bool>((bit1 & 0b000000000100000u) >> 5u);
				vob.physics_enabled = static_cast<bool>((bit1 & 0b000000001000000u) >> 6u);

				if (version == game_version::gothic_2) {
					vob.animation_mode = (bit1 & 0b000000110000000u >> 7u);
					vob.bias = static_cast<s32>((bit1 & 0b011111000000000u) >> 9u);
					vob.ambient = static_cast<bool>((bit1 & 0b100000000000000u) >> 14u);

					vob.animation_strength = bin.read_f32();
					vob.far_clip_scale = bin.read_f32();
				}

				if (has_preset_name) {
					vob.preset_name = in->read_string();
				}

				if (has_vob_name) {
					vob.vob_name = in->read_string();
				}

				if (has_visual_name) {
					vob.visual_name = in->read_string();
				}

				if (has_visual_object) {
					in->skip_object(false);
				}

				if (has_ai_object) {
					in->skip_object(false);
				}
			} else {
				vob.preset_name = in->read_string();

				auto [min, max] = in->read_bbox();
				vob.bbox[0] = min;
				vob.bbox[1] = max;

				vob.rotation = in->read_mat3x3();
				vob.position = in->read_vec3();

				vob.vob_name = in->read_string();
				vob.visual_name = in->read_string();
				vob.show_visual = in->read_bool();
				vob.camera_alignment = in->read_enum();

				if (version == game_version::gothic_1) {
					vob.cd_static = in->read_bool();
					vob.cd_dynamic = in->read_bool();
					vob.vob_static = in->read_bool();
					vob.dynamic_shadows = in->read_enum();
					// vob._m_animation_mode = in->read_byte();
				} else {
					vob.animation_mode = in->read_enum();
					vob.animation_strength = in->read_float();
					vob.far_clip_scale = in->read_float();
					vob.cd_static = in->read_bool();
					vob.cd_dynamic = in->read_bool();
					vob.vob_static = in->read_bool();
					vob.dynamic_shadows = in->read_enum();
					vob.bias = in->read_int();
					vob.ambient = in->read_bool();
				}

				// visual
				in->skip_object(false);

				// ai
				in->skip_object(false);
			}
		}
	}// namespace vob

	vob_tree vob_tree::parse(archive_reader_ref& in, game_version version) {
		vob_tree vob {};

		archive_object obj;
		if (!in->read_object_begin(obj)) {
			throw parser_error("vob_tree: expected object where there was none");
		}

		if (const auto& it = _vob_type_map.find(obj.class_name); it != _vob_type_map.end()) {
			vob._m_type = it->second;
		} else {
			throw parser_error(fmt::format("vob_tree: expected a '*:zCVob' object but got '{}'", obj.class_name));
		}

		vob::base data {};
		vob::base::parse(data, in, version);

		vob._m_content = data;

		if (!in->read_object_end()) {
			in->skip_object(true);
		}

		auto child_count = in->read_int();
		vob._m_children.reserve(child_count);

		for (int i = 0; i < child_count; ++i) {
			vob._m_children.emplace_back(vob_tree::parse(in, version));
		}

		return vob;
	}
}// namespace phoenix