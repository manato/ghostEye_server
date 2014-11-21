using System;
using System.Collections.Generic;
using System.Text;

namespace AdaBooster
{
	class Program
	{
		static void SearchFiles( string dir, string pat, System.IO.TextWriter sw, bool isRelative, bool isRecursive )
		{
			if( System.IO.Directory.Exists( dir ) )
			{
				foreach( string fname in System.IO.Directory.GetFiles( System.IO.Path.GetFullPath( dir ), pat ) )
				{
					if( isRelative )
					{
						string path = System.IO.Path.Combine( dir, System.IO.Path.GetFileName( fname ) );
						sw.WriteLine( path );
					}
					else
					{
						sw.WriteLine( fname );
					}
				}

				if( isRecursive )
				{
					foreach( string dname in System.IO.Directory.GetDirectories( dir ) )
					{
						SearchFiles( dname, pat, sw, isRelative, isRecursive );
					}
				}
			}
			else
			{
				Console.Error.WriteLine( "パス {0} が見つかりませんでした．", dir );
			}
		}

		static void Help( )
		{
			Console.WriteLine( "【ファイルリスト作成プログラム】" );
			Console.WriteLine( );

			Console.WriteLine( "[使用例]" );
			Console.WriteLine( "AdaPathGen [オプション] path1 [path2 ...]" );
			Console.WriteLine( "AdaPathGen /out out.txt path1 [path2 ...]" );
			Console.WriteLine( "※複数のパスを指定することも可能．ファイルに出力する場合は /out オプションを使用．" );
			Console.WriteLine( );

			Console.WriteLine( "[オプション]" );
			Console.WriteLine( "-relative    相対パスを出力する" );
			Console.WriteLine( "-absolute    絶対パスを出力する" );
			Console.WriteLine( "-recursive   ディレクトリを再帰的に探索する" );
			Console.WriteLine( "-out         出力ファイルを指定" );
		}

		static void Main( string[] args )
		{
			if( args.Length == 0 )
			{
				Help( );
				return;
			}

			List<string> param = new List<string>( );

			string ofilename = null;
			bool isRelative  = false;
			bool isRecursive = false;

			// パラメータを操作する
			for( int i = 0 ; i < args.Length ; i++ )
			{
				string p = args[ i ];

				if( p.StartsWith( "-" ) )
				{
					switch( p.Substring( 1 ) )
					{
					case "relative":
						isRelative = true;
						break;

					case "absolute":
						isRelative = false;
						break;

					case "recursive":
						isRecursive = true;
						break;

					case "help":
						Help( );
						return;

					case "out":
						if( i + 1 < args.Length )
						{
							ofilename = args[ i + 1 ];
							i++;
						}
						break;

					default:
						break;
					}
				}
				else
				{
					param.Add( p );
				}
			}

			try
			{
				using( System.IO.TextWriter sw = ( ofilename == null ) ? Console.Out : new System.IO.StreamWriter( ofilename, false, Encoding.GetEncoding( "shift-jis" ) ) )
				{
					foreach( string dname in param )
					{
						string dir = dname;
						if( System.IO.Directory.Exists( dir ) && ( !dir.EndsWith( @"\" ) || !dir.EndsWith( @"/" ) ) )
						{
							switch( Environment.OSVersion.Platform )
							{
							case PlatformID.Win32NT:
							case PlatformID.Win32S:
							case PlatformID.Win32Windows:
							case PlatformID.WinCE:
								dir += @"\";
								break;

							default:
								dir += @"/";
								break;
							}
						}

						string pat = System.IO.Path.GetFileName( dir );
						dir = dir.Substring( 0, dir.Length - pat.Length );

						if( pat.Length == 0 )
						{
							pat = "*";
						}

						SearchFiles( dir, pat, sw, isRelative, isRecursive );
					}
				}
			}
			catch
			{
				Console.Error.WriteLine( "ファイルリスト作成中に不明なエラーが発生しました．" );
			}
		}
	}
}
