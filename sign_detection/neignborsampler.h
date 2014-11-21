#include <mist/mist.h>
#include <mist/io/image.h>
#include <mist/utility/options.h>

#include <mist/timer.h>
#include <mist/random.h>
#include <mist/interpolate.h>





template < class Array1, class Array2 >
inline void sample_image( const Array1 &in, Array2 &out, 
	typename Array1::size_type rect_x, typename Array1::size_type rect_y, // 検出枠
	typename Array1::size_type rect_w, typename Array1::size_type rect_h,
	typename Array1::size_type image_w, typename Array1::size_type image_h, // 出力サイズ
	double min_scale, double max_scale, // 枠のスケール
	double min_r, double max_r,   // サンプリングする場所の中心からの距離
	bool norescale = false)
{

	static mist::uniform::random rnd( static_cast< int >( std::time( NULL ) ) );

	Array1::size_type width;
	Array1::size_type height;
	int new_cx;
	int new_cy;
	int new_rect_x;
	int new_rect_y;

	while ( true )
	{
		// 枠サイズの決定
		double scale = rnd.real1() * (max_scale - min_scale) + min_scale;
		//width  = image_w * scale;
		//height = image_h * scale;
		width  = rect_w * scale;
		height = rect_h * scale;

		// 枠位置の決定
		double r = (rnd.real1() * (max_r - min_r) + min_r) *  (double)rect_w;
		double theta = 3.141592 * 2.0 * rnd.real1();
		int cx = rect_x + rect_w / 2;
		int cy = rect_y + rect_h / 2;
		new_cx = r * cos( theta ) + cx;
		new_cy = r * sin( theta ) + cy;
		new_rect_x = new_cx - width / 2;
		new_rect_y = new_cy - height / 2;

		//std::cout << "false: " << new_rect_x << ", " << new_rect_y << "\n";

		if ( (new_rect_x > -1) && (new_rect_y > -1) &&
			(new_rect_x + width < in.width()) && (new_rect_y + height < in.height()) )
		{
			break;
		}
	}

	//printf("%d, (%d, %d), (%d, %d)\n", temp_num++, rect_w, rect_h, width, height); 
		

	// 切り出し
	Array1 tmp(width, height);
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			tmp(x, y) = in( new_rect_x + x, new_rect_y + y );
		}
	}

	//printf("%d, (%d, %d), (%d, %d), (%d, %d)\n", temp_num++, rect_w, rect_h, width, height); 
	
	if (norescale)
	{
		out = tmp;
	}
	else
	{
		mist::mean::interpolate(tmp, out, image_w, image_h);
		//mist::cubic::interpolate(tmp, out, rect_w, rect_h);
	}
	
}



template < class Array1 >
inline void sample_image( const Array1 &in, 
	adaboost::validation_data &out,
	typename Array1::size_type rect_x, typename Array1::size_type rect_y, // 検出枠
	typename Array1::size_type rect_w, typename Array1::size_type rect_h,
	typename Array1::size_type image_w, typename Array1::size_type image_h, // 出力サイズ
	double min_scale, double max_scale, // 枠のスケール
	double min_r, double max_r   // サンプリングする場所の中心からの距離
	)
{

	static mist::uniform::random rnd( static_cast< int >( std::time( NULL ) ) );

	Array1::size_type width;
	Array1::size_type height;
	int new_cx;
	int new_cy;
	int new_rect_x;
	int new_rect_y;

	while ( true )
	{
		// 枠サイズの決定
		double scale = rnd.real1() * (max_scale - min_scale) + min_scale;
		//width  = image_w * scale;
		//height = image_h * scale;
		width  = rect_w * scale;
		height = rect_h * scale;

		// 枠位置の決定
		double r = (rnd.real1() * (max_r - min_r) + min_r) *  (double)rect_w;
		double theta = 3.141592 * 2.0 * rnd.real1();
		int cx = rect_x + rect_w / 2;
		int cy = rect_y + rect_h / 2;
		new_cx = r * cos( theta ) + cx;
		new_cy = r * sin( theta ) + cy;
		new_rect_x = new_cx - width / 2;
		new_rect_y = new_cy - height / 2;

		//std::cout << "false: " << new_rect_x << ", " << new_rect_y << "\n";

		if ( (new_rect_x > -1) && (new_rect_y > -1) &&
			(new_rect_x + width < in.width()) && (new_rect_y + height < in.height()) )
		{
			break;
		}
	}

	//printf("%d, (%d, %d), (%d, %d)\n", temp_num++, rect_w, rect_h, width, height); 
		

	// 切り出し
	out.x = new_rect_x;
	out.y = new_rect_y;
	out.w = width;
	out.h = height;
	
}



template < class Array1 >
inline void sample_image_grid( const Array1 &in, 
	std::vector< adaboost::validation_data > &out,
	typename Array1::size_type rect_x, typename Array1::size_type rect_y, // 検出枠
	typename Array1::size_type rect_w, typename Array1::size_type rect_h,
	typename Array1::size_type image_w, typename Array1::size_type image_h, // 出力サイズ
	double min_scale, double max_scale, // 枠のスケール
	double min_r, double max_r,   // サンプリングする場所の中心からの距離
	int num = 1000
	)
{

	static mist::uniform::random rnd( static_cast< int >( std::time( NULL ) ) );

	Array1::size_type width;
	Array1::size_type height;
	int new_cx;
	int new_cy;
	int new_rect_x;
	int new_rect_y;

	out.clear();

	double step = 1.0 / sqrt((double)num);

	for (double rr = 0.0; rr <= 1.0; rr += step)
	{
		for (double ss = 0.0; ss <= 1.0; ss += step)
		{
			double scale = ss * (max_scale - min_scale) + min_scale;
			width  = rect_w * scale;
			height = rect_h * scale;

			// 枠位置の決定
			double r = (rr * (max_r - min_r) + min_r) *  (double)rect_w;
			double theta = 3.141592 * 2.0 * rnd.real1();
			int cx = rect_x + rect_w / 2;
			int cy = rect_y + rect_h / 2;
			new_cx = r * cos( theta ) + cx;
			new_cy = r * sin( theta ) + cy;
			new_rect_x = new_cx - width / 2;
			new_rect_y = new_cy - height / 2;

			if ( (new_rect_x > -1) && (new_rect_y > -1) &&
				(new_rect_x + width < in.width()) && (new_rect_y + height < in.height()) )
			{
				// 切り出し
				adaboost::validation_data v;
				v.x = new_rect_x;
				v.y = new_rect_y;
				v.w = width;
				v.h = height;
				out.push_back( v );
			}
		}
	}
	
}

