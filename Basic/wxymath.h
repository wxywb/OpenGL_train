

namespace wmath{
	class mat4{
			public:
		float d[4][4];

		mat4(){
			for (int i = 0; i <= 3; i++){
				for (int j = 0; j <= 3; j++){
					d[i][j] = 0;
				}
			}
		}
		mat4(float c[]){
			int k = 0;
			for (int i = 0; i <= 3; i++){
				for (int j = 0; j <= 3; j++){
					d[i][j] = c[k];
					k++;
				}
			}
		}
		void set(int i, int j, float value){
			d[i][j] = value;
		}
	
	};
	mat4 E(){
		mat4 E;
		E.set(0, 0, 1.0f);
		E.set(1, 1, 1.0f);
		E.set(2, 2, 1.0f);
		E.set(3, 3, 1.0f);
		return E;
	}
	mat4 translate(float x, float y, float z){
		mat4 trans;
		trans.set(0, 3, x);
		trans.set(1, 3, y);
		trans.set(2, 3, z);
		return trans;
	}

};