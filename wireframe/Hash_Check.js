
/**
	Hash_Check.js
	
	Javascript program for providing functionality
	of checking a state's processed data file's
	integrity
	
	Created 4/21/2020
*/

window.onload = function()
{
	document.getElementById('fileChecker').addEventListener('change', function()
	{        
		var reader = new FileReader(); 
		var theHash; //Object holding Hashing results
		
		reader.onload=function()
		{ 
			var fileContent= reader.result;; //Contents of file read in		
			var keySelected = document.getElementById('selectStateFile').value;
			
			switch(keySelected)
			{
				
				case 'Maryland':
				{
					//Take the file currently in directory, and encrypt 
					//!!NOTE!! Hard coding given key/password and parameters from successful encryption
					
					theHash = sjcl.encrypt("abcde", fileContent,{iv:"m0aLuLijId99uhZpe9lFnQ==",v:1,iter:10000,ks:128,ts:64,mode:"ccm",adata:"",cipher:"aes",salt:"uUcE3UaZMfQ="});
					
					//Remove useless info, keep only important things to compare
					var parsedMessage = JSON.parse(theHash);
					delete parsedMessage.mode;
					delete parsedMessage.iter;
					delete parsedMessage.ks;
					delete parsedMessage.ts;
					delete parsedMessage.v;
					delete parsedMessage.cipher;
					delete parsedMessage.salt;
					delete parsedMessage.adata;
					
					// !!NOTE!! Again, hard coding the correct values we should expect
					if(parsedMessage.iv != "m0aLuLijId99uhZpe9lFnQ==" || parsedMessage.ct != "bBdOsJmSASmI9WuB39DoyMbzielCeO2G7CtprIkNz/+liR6DIb+Dr5MORfk9q3DDv2n9XFM=")
					{
						alert("Maryland Data is Invalid!");
					}
					else
						alert("Maryland Data Valid");
					
					break;
				}
				
				case 'South Carolina':
				{
					//Do nothing, wait until processed data is ready and already hashed
					alert("South Carolina Keys not ready");
					break;
				}
				
				default:
				{
					alert("Selection Error!");
				}
			}
		} 
		reader.readAsText(this.files[0]); 
	});
}