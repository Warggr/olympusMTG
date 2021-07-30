subzone(&direction, gridz-1, !needstarget && hasroot);
		}
		else if(gridy == 1) ret = (Target*) (*pl)->metagame->iterate_stack(&direction);

		if(ret != 0){
			if(returntypeflags & (ret->target_type())){
				if(needstarget || hasroot){
					return ret;
				}
				else god.myIO->message("Can't activate your opponents's abilities");
			}
			else god.myIO->message("Wrong type of target");
		}
		else{
			switch(direction){
				case olympus::directions::LEFT: gridy -= 1; break;
				case olympus::directions::RIGHT: gridy += 1; break;
				case olympus::directions::UP: gridz += 1; break;
				case olympus::directions::DOWN: gridz -= 1; break;
				default: if(!needstarget) return 0;
			}
		}
		i
