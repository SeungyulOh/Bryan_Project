xof 0303txt 0032
template ColorRGBA {
 <35ff44e0-6c7c-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <d3e16e81-7835-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template Material {
 <3d82ab4d-62da-11cf-ab39-0020af71e433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template TextureFilename {
 <a42790e1-7810-11cf-8f52-0040333594a3>
 STRING filename;
}

template Frame {
 <3d82ab46-62da-11cf-ab39-0020af71e433>
 [...]
}

template Matrix4x4 {
 <f6f23f45-7686-11cf-8f52-0040333594a3>
 array FLOAT matrix[16];
}

template FrameTransformMatrix {
 <f6f23f41-7686-11cf-8f52-0040333594a3>
 Matrix4x4 frameMatrix;
}

template Vector {
 <3d82ab5e-62da-11cf-ab39-0020af71e433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template MeshFace {
 <3d82ab5f-62da-11cf-ab39-0020af71e433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template Mesh {
 <3d82ab44-62da-11cf-ab39-0020af71e433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

template MeshNormals {
 <f6f23f43-7686-11cf-8f52-0040333594a3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshMaterialList {
 <f6f23f42-7686-11cf-8f52-0040333594a3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material <3d82ab4d-62da-11cf-ab39-0020af71e433>]
}

template Coords2d {
 <f6f23f44-7686-11cf-8f52-0040333594a3>
 FLOAT u;
 FLOAT v;
}

template MeshTextureCoords {
 <f6f23f40-7686-11cf-8f52-0040333594a3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template XSkinMeshHeader {
 <3cf169ce-ff7c-44ab-93c0-f78f62d172e2>
 WORD nMaxSkinWeightsPerVertex;
 WORD nMaxSkinWeightsPerFace;
 WORD nBones;
}

template SkinWeights {
 <6f0d123b-bad2-4167-a0d0-80224f25fabb>
 STRING transformNodeName;
 DWORD nWeights;
 array DWORD vertexIndices[nWeights];
 array FLOAT weights[nWeights];
 Matrix4x4 matrixOffset;
}


Material SMDImport_Material__35Sub0 {
 1.000000;1.000000;1.000000;1.000000;;
 3.200000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "whitecloud01.tga";
 }
}

Frame static_prop {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }
}

Frame SMDImport {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Mesh  {
  18;
  22.476995;-172.375194;106.290803;,
  -11.403814;173.879002;317.368806;,
  -29.499613;-172.375194;317.368806;,
  -11.403814;173.879002;317.368806;,
  22.476995;-172.375194;106.290803;,
  40.572797;173.879002;106.290803;,
  22.477003;-172.375194;-104.787002;,
  40.572797;173.879002;106.290803;,
  22.476995;-172.375194;106.290803;,
  40.572797;173.879002;106.290803;,
  22.477003;-172.375194;-104.787002;,
  40.572803;173.879002;-104.787002;,
  -29.499587;-172.375194;-315.864986;,
  40.572803;173.879002;-104.787002;,
  22.477003;-172.375194;-104.787002;,
  40.572803;173.879002;-104.787002;,
  -29.499587;-172.375194;-315.864986;,
  -11.403786;173.879002;-315.864986;;
  6;
  3;0,1,2;,
  3;3,4,5;,
  3;6,7,8;,
  3;9,10,11;,
  3;12,13,14;,
  3;15,16,17;;

  MeshNormals  {
   18;
   0.969747;-0.050681;0.238794;,
   0.969747;-0.050681;0.238794;,
   0.969747;-0.050681;0.238794;,
   0.969747;-0.050681;0.238794;,
   0.969747;-0.050681;0.238794;,
   0.969747;-0.050681;0.238794;,
   0.998637;-0.052190;0.000000;,
   0.998637;-0.052190;0.000000;,
   0.998637;-0.052190;0.000000;,
   0.998637;-0.052190;0.000000;,
   0.998637;-0.052190;0.000000;,
   0.998637;-0.052190;0.000000;,
   0.969747;-0.050681;-0.238794;,
   0.969747;-0.050681;-0.238794;,
   0.969747;-0.050681;-0.238794;,
   0.969747;-0.050681;-0.238794;,
   0.969747;-0.050681;-0.238794;,
   0.969747;-0.050681;-0.238794;;
   6;
   3;0,1,2;,
   3;3,4,5;,
   3;6,7,8;,
   3;9,10,11;,
   3;12,13,14;,
   3;15,16,17;;
  }

  MeshMaterialList  {
   1;
   6;
   0,
   0,
   0,
   0,
   0,
   0;
   { SMDImport_Material__35Sub0 }
  }

  MeshTextureCoords  {
   18;
   0.666700;1.552500;,
   1.000000;1.000000;,
   1.000000;1.552500;,
   1.000000;1.000000;,
   0.666700;1.552500;,
   0.666700;1.000000;,
   0.333300;1.552500;,
   0.666700;1.000000;,
   0.666700;1.552500;,
   0.666700;1.000000;,
   0.333300;1.552500;,
   0.333300;1.000000;,
   0.000000;1.552500;,
   0.333300;1.000000;,
   0.333300;1.552500;,
   0.333300;1.000000;,
   0.000000;1.552500;,
   0.000000;1.000000;;
  }

  XSkinMeshHeader {
   1;
   0;
   1;
  }

  SkinWeights {
   "static_prop";
   18;
   0,
   1,
   2,
   3,
   4,
   5,
   6,
   7,
   8,
   9,
   10,
   11,
   12,
   13,
   14,
   15,
   16,
   17;
   1.000000,
   1.000000,
   1.000000,
   1.000000,
   1.000000,
   1.000000,
   1.000000,
   1.000000,
   1.000000,
   1.000000,
   1.000000,
   1.000000,
   1.000000,
   1.000000,
   1.000000,
   1.000000,
   1.000000,
   1.000000;
   1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
  }
 }
}