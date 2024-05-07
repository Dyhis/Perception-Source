#pragma once 
namespace framework {

	enum class request : std::uint8_t {
		m_endframe,
		m_rectangle,
		m_fill_rect,
		m_line,
		m_circle,
		m_text,
		m_boundingbox,
	};

	using vec2 = struct {
		float x, y;
	};

	using rect_t = struct {
		float height, width, x, y;
	};

	using rectangle_t = struct {

		rect_t object;
		flinearcolor color;
		vec2 radius;
		bool rounded;
		float thickness;

	};

	using bounding_t = struct {
		float height, width, x, y;
		flinearcolor color;
		bool rounded;
	};

	using line_t = struct {
		vec2 point1, point2;
		flinearcolor color;
		float thickness;
	};

	using circle_t = struct {
		vec2 point;
		float radius;
		flinearcolor color;
		float stroke;
		bool filled;
	};

	using text_t = struct {
		std::uint32_t textLength;
		vec2 point;
		flinearcolor color;
		float fontSize;
		bool center;
		bool menu;
	};

	class c_framework {

	public:

		c_framework( ) = default;

		[[nodiscard]] flinearcolor get_color( int a, int r, int g, int b );

		void end_scene( );
		void begin_scene( );

		void text( const std::string& input, float x, float y, flinearcolor color, float fontSize, bool center, bool menu = false );
		void text_font(const std::string& input, float x, float y, flinearcolor color, float fontSize, bool center, bool menu, const std::string& fontPath);
		void rect( float x, float y, float width, float height, flinearcolor color, float thickness );
		void filled_rect( float x, float y, float width, float height, flinearcolor color, bool rounded, float _x, float _y );
		void line( float x1, float y1, float x2, float y2,  flinearcolor color, float thickness );
		void circle( float x, float y, float radius, flinearcolor color, float stroke, bool filled = false );
	};

} inline std::shared_ptr<framework::c_framework> m_framework = std::make_shared<framework::c_framework>( );