/***********************************************************************
mappy.js file updated by Kavya Kavanakudy on April 15, 2020
This is the .js file that interacts with the API in order to display a 
map of a GEOJSON file provided by the algorithm
***********************************************************************/

//The source files for geojson data
var districtData ="districts.geojson"
var precinctData = "precinct2010.zip.geojson"

/****************************************
  A varialbe that stores how the 
  geojson data will be displayed based
  on party.
****************************************/

var partyStyles = {
  
  //red fill and out line for Republican districts
  'Republican': new ol.style.Style({
    fill: new ol.style.Fill({
        color: 'rgba(193, 73, 73, 0.5)'
    }),
    stroke: new ol.style.Stroke({
        color: 'rgba(204, 0, 0, 1)',
        width: 3
    })
  }),
  
  //blue fill and oultine for Democratic districts
  'Democrat': new ol.style.Style({
    fill: new ol.style.Fill({
        color: 'rgba(73, 125, 193, 0.5)'
    }),
    stroke: new ol.style.Stroke({
        color: 'rgba(0, 0, 255, 1)',
        width: 3
    }),
  })

};

/************************************************* 
  A style function for the districts
  it choose a style based on party the feature is.
  It takes in a feature and returns the approciate
  style characteristics.
***************************************************/

var styleFunction = function(feature) {
  return partyStyles[feature.get("party")]
};

/******************************************************
Creates a vector layer containing the district geojson 
data and corresponding style as defined above
******************************************************/

var districtLayer = new ol.layer.Vector({
  	source: new ol.source.Vector({
    url: districtData,
    format: new ol.format.GeoJSON(),
  }),
  style: styleFunction
});


/******************************************************
Creates a vector layer containing the precict 
geojson data and the style is just a black outline
******************************************************/

var precinctLayer = new ol.layer.Vector({
    source: new ol.source.Vector({
    url: precinctData,
    format: new ol.format.GeoJSON(),
  }),
  style: new ol.style.Style({
    stroke: new ol.style.Stroke({
      color: 'rgba(0,,0,0,1',
      width: .5
    })
  })
});

/******************************************************
The following displays the map with an 
open street maps layer, the district layer and 
precinct layer. The map is centered on Maryland.
******************************************************/

var map = new ol.Map({

    target: 'map',

    layers: [

      new ol.layer.Tile({
        source: new ol.source.OSM()
      }),
      precinctLayer,
      districtLayer
      
    ],

    view: new ol.View({
        //lon, lat coordinates for maryland
        center: ol.proj.fromLonLat([-76.641273, 39.045753]),
        zoom: 7.5
   })

});
