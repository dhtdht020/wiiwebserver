static const struct 
{
	char	*extension;
	char	*mime_type;
} 
builtin_mime_types[] =  {
	{"arj",		"application/x-arj-compressed"	},
	{"asf",		"video/x-ms-asf"				},
	{"avi",		"video/x-msvideo"				},
	{"bin",		"application/octet-stream"		},
	{"bmp",		"image/bmp"						},
	{"c",		"text/plain"					},
	{"css",		"text/css"						},
	{"doc",		"application/msword"			},
	{"exe",		"application/octet-stream"		},
	{"gif",		"image/gif"						},
	{"gtar",	"application/x-gtar"			},
	{"gz",		"application/x-gunzip"			},
	{"h",		"text/plain"					},
	{"htm",		"text/html"						},
	{"html", 	"text/html"						},
	{"ico",		"image/vnd.microsoft.icon"					},
	{"java",	"text/plain"					},
	{"jpe",		"image/jpeg"					},
	{"jpeg",	"mage/jpeg"						},
	{"jpg",		"mage/jpeg"						},
	{"js",		"application/x-javascript"		},
	{"m3u",		"audio/x-mpegurl"				},
	{"mid",		"audio/x-midi"						},	
	{"midi",	"audio/x-midi"					},
	{"mod",		"audio/mod"						},
	{"mp2",		"video/mpeg"					},
	{"mp3",		"audio/mpeg"					},
	{"mpa",		"video/mpeg"					},
	{"mpe",		"video/mpeg"					},
	{"mpeg",	"video/mpeg"					},
	{"mpg",		"video/mpeg"					},
	{"mpp",		"application/vnd.ms-project"	},
	{"mpv2",	"video/mpeg"					},
	{"mp3",		"audio/x-mp3"					},
	{"mov",		"video/quicktime"				},
	{"pdf",		"application/pdf"				},
	{"png",		"image/png"						},
	{"pps",		"application/vnd.ms-powerpoint"	},
	{"ppt",		"application/vnd.ms-powerpoint"	},
	{"qt",		"video/quicktime"				},
	{"ra",		"audio/x-pn-realaudio"			},
	{"ram",		"audio/x-pn-realaudio"			},
	{"rar",		"application/x-arj-compressed"	},
	{"rtf",		"application/rtf"				},
	{"rtx",		"text/richtext"					},
	{"svg",		"image/svg+xml"					},
	{"swf",		"application/x-shockwave-flash"	},
	{"tar",		"application/x-tar"				},
	{"tif",		"image/tiff"					},
	{"tiff",	"image/tiff"					},
	{"txt",		"text/plain"					},
	{"tgz",		"application/x-tar-gz"			},
	{"torrent",	"application/x-bittorrent"		},
	{"txt",		"text/plain"					},
	{"wav",		"audio/x-wav"					},
	{"wmv",		"video/x-ms-wmv"    },
	{"xls",		"application/excel"				},
	{"xla",		"application/vnd.ms-excel"		},
	{"xlc",		"application/vnd.ms-excel"		},
	{"xlm",		"application/vnd.ms-excel"		},
	{"xls",		"application/vnd.ms-excel"		},
	{"xlt",		"application/vnd.ms-excel"		},
	{"xlw",		"application/vnd.ms-excel"		},
	{"xhtml", "application/xhtml+xml"},
	{"zip",		"application/x-zip-compressed"	},
	{NULL,		NULL				}
};
