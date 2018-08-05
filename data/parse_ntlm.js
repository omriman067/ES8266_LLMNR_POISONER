let ParseNTLMHash = function(input){
	input = input.replace(' ','').replace('\n', '').replace('\r','');
	data = atob(input);
	const Challenge = '0011223344556677';
	function unpack(str) {
		 let bytes = [];
		 for(let i = 0; i < str.length; i++) {
			 let char = str.charCodeAt(i);
			 bytes.push(char >>> 8);
			 bytes.push(char & 0xFF);
		 }
		 return bytes[1] + (256 * bytes[3]);
	}


	function hexEncode(str){
		let hex, i;
		let result = "";
		for (i=0; i<str.length; i++) {
			hex = str.charCodeAt(i).toString(16);
			if(hex.length < 2) {
				hex = '0' + hex;
			}
			result += hex;
		}
		return result
	}
	let LMhashLen = unpack(data.substr(14,2));
	let LMhashOffset = unpack(data.substr(16,2));
	let LMHash = hexEncode(data.substr(LMhashOffset, LMhashLen)).toUpperCase();
	let NthashLen = unpack(data.substr(22,2));
	let NthashOffset = unpack(data.substr(24,2));

	if(NthashLen == 24){
		let NtHash = hexEncode(data.substr(NthashOffset, NthashLen)).toUpperCase();
		let DomainLen = unpack(data.substr(30,2));
		let DomainOffset = unpack(data.substr(32,2));
		let Domain = data.substr(DomainOffset, DomainLen).replace(/[^ -~]+/g, "").replace('\n','').replace('\r','');
		let UserLen = unpack(data.substr(38,2));
		let UserOffset = unpack(data.substr(40,2));
		let User = data.substr(UserOffset, UserLen).replace(/[^ -~]+/g, "").replace('\n','').replace('\r','');
		let writehash = User+"::"+Domain+":"+LMHash+":"+NtHash+":"+Challenge;
		return writehash+"\n";
	} else if (NthashLen > 60) {
		let NtHash = hexEncode(data.substr(NthashOffset, NthashLen)).toUpperCase();
		let DomainLen = unpack(data.substr(30,2));
		let DomainOffset = unpack(data.substr(32,2));
		let Domain = data.substr(DomainOffset, DomainLen).replace(/[^ -~]+/g, "").replace('\n','').replace('\r','');
		let UserLen = unpack(data.substr(38,2));
		let UserOffset = unpack(data.substr(40,2));
		let User = data.substr(UserOffset, UserLen).replace(/[^ -~]+/g, "").replace('\n','').replace('\r','');

		writehash = User+"::"+Domain+":"+Challenge+":"+NtHash.substr(0,32)+":"+NtHash.substr(32);
		return writehash+"\n";
	}
	return false;
}