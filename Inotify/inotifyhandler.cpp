
#include <inotifyhandler.h>
#include <exception>

InotifyHandler::InotifyHandler()
{
}

void InotifyHandler::initialize_inotify(){
    root_dir=Util::get_root_path();
    string watch_dir=this->root_dir;

    try{


        InotifyWatch watch(watch_dir, IN_CREATE |IN_DELETE |IN_MODIFY|IN_MOVE);
        notify.Add(watch);
        string path =watch_dir;
        this->recursively_add_watch(path);
        this->wait_for_events();

    }
    catch (InotifyException &e) {
        cerr << "Inotify exception occured: " << e.GetMessage() << endl;
    } catch (exception &e) {
        cerr << "STL exception occured: " << e.what() << endl;
    } catch (...) {
        cerr << "unknown exception occured" << endl;
    }
}
/**
 * @brief InotifyHandler::wait_for_events
 * wait for events -file create, delete, rename,move, modify
 *                  -dir create , delete,reanme , move
 *
 * and call file handler for handle these events
 */
void InotifyHandler::wait_for_events(){


    string moved_from_dir="";
    string moved_into_dir="";
    string fname_before_move="";
    string fname_after_move="";

    string dmoved_from_dir="";
    string dmoved_into_dir="";
    string dname_before_move="";
    string dname_after_move="";
    string watchDir;
    for (;;) {
        notify.WaitForEvents();


        size_t count = notify.GetEventCount();

        while (count > 0) {

            InotifyEvent event;
            bool got_event = notify.GetEvent(&event);

            if (got_event) {
                string mask_str;
                event.DumpTypes(mask_str);

                string filename = event.GetName();

                watchDir=event.GetWatch()->GetPath ();

                //   cout << "[Event in " << watchDir << "] ";
                // cout << "event mask: \"" << mask_str << "\", ";
                //cout << "filename: \"" << filename << "\"" << endl;

                /**** file created  and deleted messages********/
                if(mask_str == "IN_CREATE"){// file created
                    if(filename.find(".goutputstream")== string::npos && filename.find("~")== string::npos){//temp file
                        cout << "[Event in " << watchDir << "] ";
                        cout << "New File Created: \"" << filename << "\"" << endl;

                        boost::thread workerThread(&InotifyHandler::file_created_checker,this,watchDir+"/"+filename);
                        workerThread.join();

                    }

                }
                if(mask_str == "IN_DELETE"){// for shift+delete button
                    if(filename.find("~")== string::npos){
                        cout << "[Event in " << watchDir << "] ";
                        cout << " File Deleted: \"" << filename << "\"" << endl;
                        fh.file_delete(Util::get_relative_path(watchDir),filename);

                    }
                }
                if(mask_str == "IN_MOVED_FROM" && count==1){ // for delte button
                    /*  if(filename.find(".goutputstream")== string::npos){
                    cout << "[Event in " << watchDir << "] ";
                    cout << " IN_MOVED_FROM File Deleted: \"" << filename << "\"" << endl;
                  //  fh.file_delete(Util::get_relative_path(watchDir),filename);
}*/
                }
                /***end*********/

                /** add watch for new direcotry && remove watch for deleted directory**/
                if(mask_str == "IN_CREATE,IN_ISDIR"){		// new direcotry is created
                    string new_dir_name=event.GetName();		// name of new directory
                    cout << "[Event in " << watchDir << "] ";
                    cout << "New Direcotry Created: \"" << new_dir_name << "\"" << endl;
                   /* if(new_dir_name.find("Untitled Folder")!= string::npos){
                        add_watch(watchDir+"/"+new_dir_name);
                        cout << "yo yo :" +Util::get_relative_path(watchDir) <<endl;
                        fh.dir_created(Util::get_relative_path(watchDir),new_dir_name);
                    }else{*/
                        string inode=CMD::get_inode_number(watchDir+"/"+new_dir_name);
                        boost::thread workerThread(&InotifyHandler::dir_created_checker,this,inode);
                       workerThread.join();
                  /*  }*/


                }


                if(mask_str == "IN_DELETE,IN_ISDIR"){// direcotry deleted
                    string deletedDirName=event.GetName();
                    cout << "[Event in " << watchDir << "] ";
                    cout << "Direcotry Deleted: \"" << deletedDirName << "\"" << endl;

                    notify.Remove(notify.FindWatch(watchDir+"/"+deletedDirName));
                    fh.dir_deleted(Util::get_relative_path(watchDir),filename);
                }
                if(mask_str== "IN_MOVED_FROM,IN_ISDIR" && count==1){

                    /*      string deletedDirName=event.GetName();
                    cout << "[Event in " << watchDir << "] ";
                    cout << "Direcotry Deleted: \"" << deletedDirName << "\"" << endl;

                    notify.Remove(notify.FindWatch(watchDir+"/"+deletedDirName));*/

                }
                /******end***********/


                /** file rename ,modify and move and created**/
                if(mask_str == "IN_MOVED_FROM"){
                    moved_from_dir=watchDir;
                    fname_before_move=event.GetName();

                }

                if(mask_str == "IN_MOVED_TO"){
                    moved_into_dir=watchDir;
                    fname_after_move=filename;
                    if(moved_from_dir!=""){
                        if(moved_from_dir==moved_into_dir &&fname_before_move!=fname_after_move){// modified
                            if(fname_before_move.find(".goutputstream")!= string::npos){
                                cout << "[Event in " << moved_from_dir << "] ";
                                cout << "File modified : " +fname_after_move<< endl;
                                fh.file_modified(Util::get_relative_path(moved_into_dir),fname_after_move);
                                moved_from_dir="";
                                moved_into_dir="";
                                fname_before_move="";
                                fname_after_move="";

                            }

                            else{// renamed
                                cout << "[Event in " << moved_from_dir << "] ";
                                cout << "File " +fname_before_move+ " is renamed as " +fname_after_move<< endl;
                                fh.file_rename(Util::get_relative_path(moved_from_dir),fname_before_move,fname_after_move);
                                moved_from_dir="";
                                moved_into_dir="";
                                fname_before_move="";
                                fname_after_move="";
                            }
                        }   else{// moved
                            cout << "File "+fname_after_move+" has been moved from "+moved_from_dir+" to "+moved_into_dir<<endl;

                            fh.file_moved(Util::get_relative_path(moved_from_dir),Util::get_relative_path(moved_into_dir),fname_after_move);
                            moved_from_dir="";
                            moved_into_dir="";
                            fname_before_move="";
                            fname_after_move="";
                        }
                    }else {
                        cout << "file created"<< endl; // to do file created
                    }

                }
                /*****end****/


                /** dir rename and move and created**/

                if(mask_str== "IN_MOVED_FROM,IN_ISDIR"){

                    dmoved_from_dir=event.GetWatch()->GetPath ();

                    dname_before_move=event.GetName();
                    try{
                        if(notify.FindWatch(dmoved_from_dir+"/"+dname_before_move)!=NULL){
                            notify.Remove(notify.FindWatch(dmoved_from_dir+"/"+dname_before_move));//remove watch from the folder (to add new watch after remane/move)
                            cout << "remove watch called for main folder"<< endl;
                            // to do remove recursivly
                        }
                    }catch(InotifyException e){
                        cout<<"watch remove faile"<< endl;
                    }
                }

                if(mask_str == "IN_MOVED_TO,IN_ISDIR"){
                    dmoved_into_dir=event.GetWatch()->GetPath ();

                    dname_after_move=event.GetName();
                    if(dmoved_from_dir!=""){
                        if(dmoved_from_dir==dmoved_into_dir &&dname_before_move!=dname_after_move){
                            cout << "[Event in " << dmoved_from_dir << "] ";
                            cout << "Directory " +dname_before_move+ " is renamed as " +dname_after_move<< endl;

                            string new_watchDir=dmoved_into_dir+"/"+dname_after_move;
                            try{
                                if(notify.FindWatch(new_watchDir)==NULL){

                                    notify.Add(new InotifyWatch(new_watchDir, IN_CREATE |IN_DELETE |IN_MODIFY|IN_MOVE)); // add watch to renamed folder
                                   // cout << "add watch called in rename "<< endl;
                                }
                            }catch (InotifyException e){

                                cout <<e.GetMessage()<<endl;
                            }
                            fh.dir_rename(Util::get_relative_path(dmoved_from_dir),dname_before_move,dname_after_move);
                            dmoved_from_dir="";
                            dmoved_into_dir="";
                            dname_before_move="";
                            dname_after_move="";
                        }else{
                            cout << "Drectory "+dname_after_move+" has been moved from "+dmoved_from_dir+" to "+dmoved_into_dir<<endl;


                            try{
                                if(notify.FindWatch(dmoved_into_dir+"/"+dname_after_move)==NULL){
                                    notify.Add(new InotifyWatch(dmoved_into_dir+"/"+dname_after_move, IN_CREATE |IN_DELETE |IN_MODIFY|IN_MOVE)); //add wathc to moved folder
                                    cout << "add watch called for main folder"<< endl;
                                }
                            }catch(exception&  e){
                                cout<<"erro in add inotify while move:"<<endl;
                            }
                            //fh.dir_moved(Util::get_relative_path(moved_from_dir)+fname_before_move,Util::get_relative_path(moved_into_dir)+fname_after_move);
                            //	 RecursiveRemoveWatchInMove(notify,moved_into_dir+"/"+fname_after_move,moved_from_dir+"/"+fname_before_move);

                            string cparentdir=dmoved_into_dir+"/"+dname_after_move;
                            string path =cparentdir;
                            boost::filesystem::recursive_directory_iterator itr(path);
                            string pfilewatchpath="";
                            string temp="";
                            while (itr != boost::filesystem::recursive_directory_iterator())// recursive add watch for root
                            {
                                if(is_directory(itr->path())){
                                    temp=itr->path().string();
                                    // unsigned pos=temp.find(fname_after_move);
                                    // std::cout <<"current path :"<< temp << std::endl;
                                    pfilewatchpath=dmoved_from_dir+"/"+temp.substr(dmoved_into_dir.length()+1);
                                    //std::cout <<"moved_from_dir :"<< moved_from_dir << std::endl;
                                    // std::cout <<"temp.substr(pos) :"<< temp.substr(moved_into_dir.length()+1) << std::endl;
                                    // std::cout <<"previous path :"<< pfilewatchpath << std::endl;
                                    try{
                                        if(notify.FindWatch(pfilewatchpath)!=NULL){
                                            notify.Remove(notify.FindWatch(pfilewatchpath));
                                           // cout << "remove watch called for sub folder"<< endl;
                                        }
                                        //cout << hpfilewatchpath << endl;
                                        if(notify.FindWatch(temp)==NULL){
                                            notify.Add(new InotifyWatch(temp, IN_CREATE |IN_DELETE |IN_MODIFY|IN_MOVE));
                                          //  cout << "add watch called for sub folder"<< endl;
                                        }
                                    }catch(exception& e){
                                        cout<<"error in child:"<<endl;
                                    }
                                }
                                ++itr;

                            }

                            fh.dir_moved(Util::get_relative_path(dmoved_from_dir)+dname_before_move,Util::get_relative_path(dmoved_into_dir)+dname_after_move);// move only main dir so in server all child should move
                            dmoved_from_dir="";
                            dmoved_into_dir="";
                            dname_before_move="";
                            dname_after_move="";

                        }

                    }else{//dir created
                    }
                }
                /*****end******/

            }

            count--;
        }
    }
}


/**
 * @brief InotifyHandler::add_watch
 *add inotify watch for a directory if already it has not been added
 * @param path
 */
void InotifyHandler::add_watch(string path){
    if(!notify.FindWatch(path)){
        notify.Add(new InotifyWatch(path, IN_CREATE |IN_DELETE |IN_MODIFY|IN_MOVE));
    }

}

/**
 * @brief InotifyHandler::recursively_add_watch
 *this function will iterate through a given directory and will add inotify watch to all its sub directories
 * @param dir
 * parent direcotory path
 */
void InotifyHandler::recursively_add_watch(string dir){
    string path =dir;
    boost::filesystem::recursive_directory_iterator itr(path);

    while (itr != boost::filesystem::recursive_directory_iterator())// recursive add watch for root
    {
        if(boost::filesystem3::is_directory(itr->path())){
            //std::cout << itr->path()<< std::endl;
            //std::cout << itr->path().parent_path() << std::endl;

            //notify.Add(new InotifyWatch(itr->path().string(), IN_CREATE |IN_DELETE |IN_MODIFY|IN_MOVE));
            add_watch(itr->path().string());


        }
        ++itr;

    }
}


/**
 * @brief InotifyHandler::DirCreatedChecker
 *thread function
 *checck whether a directory is completely created(including files and sub dirs)
 * @param path
 *path of the directory
 * @return
 */
void InotifyHandler::dir_created_checker(string inode){
    boost::posix_time::seconds Time(4);
    size_t previous_size=-1;
    size_t current_size=-1;
    //add_watch(path);
    boost::this_thread::sleep(Time);// wait for renaming
    string path=CMD::get_path_from_inode(inode);
    previous_size=Util::get_dir_size(path);// not thread safe

    while(true){

        path=CMD::get_path_from_inode(inode);
        current_size=Util::get_dir_size(path);
        if(previous_size==current_size){// dir completely created
            //std::cout<< current_size<< std::endl;
            break;
        }else {
            previous_size=current_size;
        }
        boost::this_thread::sleep(Time);// wait for copying
    }
    std::cout<< "dir completely created"<< std::endl;
    //std::cout<< path<< std::endl;
    // to do -lock  parent dir
    add_watch(path);
    //recursively_add_watch(path);

    string temp_path=Util::get_parent_path(path);
    string temp_dirname=Util::get_filename_from_path(path);
    fh.dir_created(Util::get_relative_path(temp_path),temp_dirname);

    boost::filesystem::recursive_directory_iterator itr(path);

    while (itr != boost::filesystem::recursive_directory_iterator())// recursive add watch for root
    {
        if(boost::filesystem3::is_directory(itr->path())){
            std::cout << "dir created :" << itr->path()<< std::endl;

            add_watch(itr->path().string());
            temp_path=Util::get_parent_path(itr->path().string());
            temp_dirname=Util::get_filename_from_path(itr->path().string());
            //cout <<Util::get_relative_path(temp_path)<< endl;
            //cout <<temp_dirname<< endl;
            fh.dir_created(Util::get_relative_path(temp_path),temp_dirname);

        }else {
            std::cout << "file created :" << itr->path()<< std::endl;
            temp_path=Util::get_parent_path(itr->path().string());
            temp_dirname=Util::get_filename_from_path(itr->path().string());
            fh.file_create(Util::get_relative_path(temp_path),temp_dirname);
        }
        ++itr;

    }
    // to do -release lock

}


/**
 * @brief InotifyHandler::FileCreatedChecker
 *thread function
 *check whether a file is completely created
 * @param path
 * path of the file
 * @return
 */
void InotifyHandler::file_created_checker(string path){

    boost::posix_time::seconds Time(4);
    size_t previous_size=-1;
    size_t current_size=-1;

    previous_size=Util::get_file_size(path);

    while(true){

        boost::this_thread::sleep(Time);
        current_size=Util::get_file_size(path);
        if(previous_size==current_size){// file completely created
            break;
        }else {
            previous_size=current_size;
        }
    }

    // call file handler
    std::cout<< "file completely created"<< std::endl;
    string temp_path=Util::get_parent_path(path);
    string temp_filename=Util::get_filename_from_path(path);
    fh.file_create(Util::get_relative_path(temp_path),temp_filename);
}






