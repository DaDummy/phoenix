// Copyright © 2022 Luis Michaelis <lmichaelis.all+dev@gmail.com>
// SPDX-License-Identifier: MIT
#pragma once
#include <phoenix/proto_mesh.hh>
#include <phoenix/softskin_mesh.hh>

namespace phoenix {
	/// \brief Represents a *ZenGin* model mesh.
	///
	/// <p>Model meshes contain multiple phoenix::softskin_mesh instances as well as a set of phoenix::proto_mesh
	/// attachments. They can be found within `MDM` files and are embedded within phoenix::model objects.</p>
	class model_mesh {
	public:
		/// \brief Parses a model mesh from the data in the given buffer.
		///
		/// <p>This implementation is heavily based on the implementation found in
		/// [ZenLib](https://github.com/Try/ZenLib).</p>
		///
		/// \param[in,out] buf The buffer to read from.
		/// \return The parsed model mesh object.
		/// \note After this function returns the position of \p buf will be at the end of the parsed object.
		///       If you would like to keep your buffer immutable, consider passing a copy of it to #parse(buffer&&)
		///       using buffer::duplicate.
		/// \throws parser_error if parsing fails.
		/// \see #parse(buffer&&)
		[[nodiscard]] static model_mesh parse(buffer& buf);

		/// \brief Parses a model mesh from the data in the given buffer.
		/// \param[in] buf The buffer to read from (by rvalue-reference).
		/// \return The parsed model mesh object.
		/// \throws parser_error if parsing fails.
		/// \see #parse(buffer&&)
		[[nodiscard]] inline static model_mesh parse(buffer&& buf) {
			return model_mesh::parse(buf);
		}

		/// \return A list of soft-skin meshes associated with this model mesh.
		const std::vector<softskin_mesh>& meshes() const noexcept {
			return _m_meshes;
		}

		/// \return A map of attachment names to attachment meshes of this model mesh.
		const std::unordered_map<std::string, proto_mesh>& attachments() const noexcept {
			return _m_attachments;
		}

	private:
		std::vector<softskin_mesh> _m_meshes;
		std::unordered_map<std::string, proto_mesh> _m_attachments;
	};
} // namespace phoenix
