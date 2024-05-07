#pragma once 
namespace utilities
{
	class c_utilities
	{

	public:

		const std::wstring get_gamebar_sid( std::uint32_t process_id );
		const std::uint32_t get_process_id( const std::wstring_view process_name );
	};

} inline std::shared_ptr<utilities::c_utilities> m_util = std::make_shared<utilities::c_utilities>( );