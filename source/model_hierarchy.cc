// Copyright © 2021 Luis Michaelis
// Licensed under MIT (https://mit-license.org/).
#include <phoenix/model_hierarchy.hh>

#include <fmt/format.h>

namespace phoenix {
	enum class hierarchy_chunk { unknown, hierarchy = 0xD100, stats = 0xD110, end = 0xD120 };

	model_hierachy model_hierachy::parse(buffer& in) {
		model_hierachy hierarchy;

		hierarchy_chunk type = hierarchy_chunk::unknown;
		bool end_hierarchy = false;

		do {
			type = static_cast<hierarchy_chunk>(in.get_ushort());

			auto length = in.get_uint();
			auto chunk = in.extract(length);

			switch (type) {
			case hierarchy_chunk::hierarchy: {
				(void) /* version = */ chunk.get_uint();
				auto node_count = chunk.get_ushort();

				for (int i = 0; i < node_count; ++i) {
					auto& node = hierarchy._m_nodes.emplace_back();
					node.name = chunk.get_line(false);

					node.parent_index = chunk.get_short();
					node.parent = node.parent_index == -1 ? nullptr : &hierarchy._m_nodes[node.parent_index];

					node.transform = chunk.get_mat4x4();
				}

				hierarchy._m_bbox = bounding_box::parse(chunk);
				hierarchy._m_collision_bbox = bounding_box::parse(chunk);
				hierarchy._m_root_translation = chunk.get_vec3();

				(void) /* checksum = */ chunk.get_uint();
				break;
			}
			case hierarchy_chunk::stats:
				// maybe a date?
				chunk.skip(16);
				(void) /* path? = */ chunk.get_line(false);
				break;
			case hierarchy_chunk::end:
				end_hierarchy = true;
				break;
			default:
				break;
			}

			if (chunk.remaining() != 0)
				fmt::print(stderr, "warning: model hierarchy: not all data consumed from section 0x{:X}\n", type);
		} while (!end_hierarchy);

		return hierarchy;
	}
} // namespace phoenix
