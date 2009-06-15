--===========================================================================================================
-- This is a lua script for generating boiler plate C++ code 
--   (file header comments, includes, class definitions, etc)
--===========================================================================================================

ITEM_NAME = arg[1];
ITEM_EXT  = arg[2];
ITEM_PATH  = arg[3];
PROJECT_FILE_NAME = arg[4];

if not ITEM_NAME or 
   not ITEM_EXT  or 
   not ITEM_PATH or
   not PROJECT_FILE_NAME then
    print("ARGS MUST BE: <item_name> <item_ext> <item_path> <project_file_name>.  Make sure the file is in the project!")
    os.exit(0)
end


-- ====== GLOBAL SETTINGS.  EDIT THESE TO CONFIGURE THE SCRIPT ===============

INDENT_AMOUNT   = 4   -- number of spaces to indent
DOXYGEN_GROUP   = "TinyRT"
NAMESPACE_NAME  = "TinyRT"    -- namespace declaration to generate.  Use nil for no namespace
FILE_PREFIX     = "TRT"       -- Prefix used on source files.  This is stripped off of the filename to create the class name
PROJECT_DESC    = "Part of the TinyRT raytracing library."
AUTHOR          = "Author: Joshua Barczak"
COPYRIGHT       = "Copyright 2009 Joshua Barczak.  All rights reserved."

LICENSE = "//   See  Doc/LICENSE.txt for terms and conditions."

-- Table of #include directives to insert into source files.  The string [CLASS_HEADER] means generate one based on the item name   

INCLUDES = {
    "[CLASS_HEADER]"
};




--===========================================================================================================
--===========================================================================================================
--===========================================================================================================


function PutText( sText )
    
    local txt = sText;
    if INDENTATION then
        for i=0,INDENTATION-1 do
            txt = " " .. txt
        end
    end
    file:write( txt .. "\n" )
    
end

function EqualBar()
    PutText("//=====================================================================================================================" )
end

--===========================================================================================================
function GetClassName( )
    if FILE_PREFIX then
        return string.sub( ITEM_NAME, 1+string.len( FILE_PREFIX ), string.len(ITEM_NAME) )
    else
        return ITEM_NAME;
    end
end

--===========================================================================================================
function HeaderFileComment()

    EqualBar()
    PutText("//")
    PutText("//   " .. ITEM_NAME .. "." .. ITEM_EXT )
    PutText("//")
  if NAMESPACE_NAME then
    PutText("//   Definition of class: " .. NAMESPACE_NAME .. "::" .. GetClassName() );
  else
    PutText("//   Definition of class: " .. ITEM_NAME );
  end
    
    PutText("//")
    PutText("//   " .. PROJECT_DESC )
    PutText("//   " .. AUTHOR )
    PutText("//   " .. COPYRIGHT )
    PutText("//")
    PutText( LICENSE )
    PutText("//")
    EqualBar()

end

--===========================================================================================================
function GenerateInclude( include )

    if include == "[CLASS_HEADER]" then
        -- special case, #include this class's header
        PutText("#include \"" .. ITEM_NAME .. ".h\"" );
    else
        PutText("#include \"" .. include .. "\"")
    end
   
end

--===========================================================================================================
 function SourceFileComment()

    EqualBar()
    PutText("//")
    PutText("//   " .. ITEM_NAME .. "." .. ITEM_EXT )
    PutText("//")
  if NAMESPACE_NAME then
    PutText("//   Definition of class: " .. NAMESPACE_NAME .. "::" .. GetClassName() );
  else
    PutText("//   Definition of class: " .. GetClassName() );
  end
    
    PutText("//")
    PutText("//   " .. PROJECT_DESC )
    PutText("//   " .. AUTHOR )
    PutText("//   " .. COPYRIGHT )
    PutText("//")
    PutText( LICENSE )
    PutText("//")
    EqualBar()


end

--===========================================================================================================
function BeginNamespace()
    if NAMESPACE_NAME then
        PutText("namespace " .. NAMESPACE_NAME )
        PutText("{")
        PutText("")
        INDENTATION = INDENT_AMOUNT
    end
end

--===========================================================================================================
function EndNamespace()

    if NAMESPACE_NAME then
        INDENTATION = nil
        PutText("}")
        PutText("")
    end

end

--===========================================================================================================
 function MakeHeaderFile()
    local sIncludeGuard =  "_" .. string.upper(ITEM_NAME) .. "_H_"

    HeaderFileComment()
    PutText("")
    PutText("#ifndef " .. sIncludeGuard)
    PutText("#define " .. sIncludeGuard)
    PutText("")
    PutText("")
    
    BeginNamespace()    

    EqualBar()
    PutText("/// \\ingroup " .. DOXYGEN_GROUP )
    PutText("/// \\brief ")
    EqualBar()
    
    PutText("class " .. GetClassName() );
    PutText("{")
    PutText("public:")
    PutText("")
    PutText("};")
    PutText("")

    EndNamespace()
        
    PutText("#endif // " .. sIncludeGuard)
    
end

--===========================================================================================================
function MakeSourceFile()

    SourceFileComment()
    PutText("")

    if INCLUDES then
        for i=1,table.getn(INCLUDES) do
            GenerateInclude( INCLUDES[i] );
        end
    end
    
    PutText("")
    BeginNamespace()
    PutText("")

    EqualBar()
    PutText("//")
    PutText("//         Constructors/Destructors")
    PutText("//")
    EqualBar()
    PutText("")

    EqualBar()
    PutText("//")
    PutText("//            Public Methods")
    PutText("//")
    EqualBar()
    PutText("")

    EqualBar()
    PutText("//")
    PutText("//           Protected Methods")
    PutText("//")
    EqualBar()
    PutText("")

    EqualBar()
    PutText("//")
    PutText("//            Private Methods")
    PutText("//")
    EqualBar()
    
    EndNamespace()

end


-- Table which tells the script what to do for each file extension.  This is used for mapping file extensions (.C, .cxx, .c, etc) onto
--  source file types (header, code, inline)

DISPATCH_TABLE = { 
   [".cpp"] = MakeSourceFile ,
   [".h"]   = MakeHeaderFile ,
   [".inl"] = MakeSourceFile 
}

file, errorMessage = io.open( ITEM_PATH, "a" );

if not file then
    print( errorMessage );
    os.exit(0);
end

if DISPATCH_TABLE[ ITEM_EXT ] then
    DISPATCH_TABLE[ ITEM_EXT ]()
else
    print("DON'T KNOW WHAT TO DO WITH THIS EXTENSION!" .. ITEM_EXT)
end


file:close();