using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;
using System.Net.Sockets;
using System.Threading;
using DSC.ClientPermissions;
using Quartz;
using System.Security.Cryptography;
//! \file ClientCommands.cs Contains command-parsing porition of ClientHandler class.

/* 
 * Contact: Reo - reobaird@gmail.com
 * Revision: 2
 * Date: June 6th
 * 
 * Changelog:
 * 
 * June 6: Added usr branch for presets and moved preset logic out of cam module.
 * 
 * */
namespace DSC
{
    //Summary written in clienthandler.cs
    partial class ClientHandler
    {
        // any command function must match this interface (same return type and arguments)
        delegate void CommandDelegate(string[] args);

        #region Root level commands
        private Hashtable rootCmds;
        /// <summary>
        /// Initialize root-level commands hashtable. This function is called the first time
        /// a client sends a command.
        /// </summary>
        private void InitRootCmds()
        {
            Log.Instance.Write("Initializing root cmds hashtable", Log.Priority.VERBOSE_DEBUG);
            this.rootCmds = new Hashtable();
            this.rootCmds.Add("sys", new CommandDelegate(this.Root_SysCmdFunc));
            this.rootCmds.Add("usr", new CommandDelegate(this.Root_UsrCmdFunc));
            this.rootCmds.Add("cam", new CommandDelegate(this.Root_CamCmdFunc));
            this.rootCmds.Add("pt", new CommandDelegate(this.Root_PTCmdFunc));
            this.rootCmds.Add("rly", new CommandDelegate(this.Root_RlyCmdFunc));
            this.rootCmds.Add("sch", new CommandDelegate(this.Root_SchCmdFunc));
            this.rootCmds.Add("help", new CommandDelegate(this.Root_HelpCmdFunc));
            this.rootCmds.Add("dc", new CommandDelegate(this.Root_DcCmdFunc));
            this.rootCmds.Add("login", new CommandDelegate(this.Root_LoginFunc));
            this.rootCmds.Add("control", new CommandDelegate(this.Root_ControlFunc));
            this.rootCmds.Add("status", new CommandDelegate(this.Root_StatusFunc));
        }
        /// <summary>
        /// Entry point for command parsing.
        /// </summary>
        /// <param name="args">args</param>
        private void ProcessRootCommand(string[] args)
        {
            if (rootCmds.ContainsKey(args[0]))
                try
                {
                    ((CommandDelegate)rootCmds[args[0]])(args);
                }
                catch(Exception e)
                {
                    lock (this._TCPLock)
                        TCPServer.Send(MSG.BADCMD, this.Socket);
                    Log.Instance.Write("Command Exception: " + e.ToString(),Log.Priority.ERROR);
                }

            else
            {
                lock (this._TCPLock)
                    TCPServer.Send(MSG.BADCMD, this.Socket);
                

            }
        }
        /// <summary>
        /// Move to sys hashtable
        /// </summary>
        /// <param name="args">args</param>
        private void Root_SysCmdFunc(string[] args)
        {
            if (this.group.accessLevel != AccessLevels.ADMIN)
            {
                lock(_TCPLock)
                    TCPServer.Send(MSG.PFAIL, this.Socket);
                return;
            }
            if (this.sysCmds == null)
                this.InitSysCmds();
            if (args.Length >= 2)
                this.ProcessSysCommand(args);
            else
                TCPServer.Send(MSG.BADCMD, this.Socket);
        }
        /// <summary>
        /// Move to usr hashtable
        /// </summary>
        /// <param name="args">args</param>
        private void Root_UsrCmdFunc(string[] args)
        {
            if (this.group.accessLevel < AccessLevels.USER)
            {
                lock (_TCPLock)
                    TCPServer.Send(MSG.PFAIL, this.Socket);
                return;
            }
            if (this.usrCmds == null)
                this.InitUsrCmds();
            if (args.Length >= 2)
                this.ProcessUsrCommand(args);
            else
                TCPServer.Send(MSG.BADCMD, this.Socket);
        }
        /// <summary>
        /// Move to cam hashtable
        /// </summary>
        /// <param name="args">args</param>
        private void Root_CamCmdFunc(string[] args)
        {
            if (!DirectControl.CheckDirectControl(this.clientID))
            {
                lock (_TCPLock)
                    TCPServer.Send(MSG.CFALSE, this.Socket);
                return;
            }
            if (this.camCmds == null)
                this.InitCamCmds();
            if (args.Length >= 2)
                this.ProcessCamCommand(args);
            else
                TCPServer.Send(MSG.BADCMD, this.Socket);
        }
        /// <summary>
        /// Move to PT hashtable
        /// </summary>
        /// <param name="args">args</param>
        private void Root_PTCmdFunc(string[] args)
        {
            if (!DirectControl.CheckDirectControl(this.clientID))
            {
                lock (_TCPLock)
                    TCPServer.Send(MSG.CFALSE, this.Socket);
                return;
            }
            if (this.ptCmds == null)
                this.InitPtCmds();
            if (args.Length >= 2)
                this.ProcessPtCommand(args);
            else
                TCPServer.Send(MSG.BADARG, this.Socket);
        }
        /// <summary>
        /// Move to rly hashtable
        /// </summary>
        /// <param name="args">args</param>
        private void Root_RlyCmdFunc(string[] args)
        {
            if (!DirectControl.CheckDirectControl(this.clientID))
            {
                lock (_TCPLock)
                    TCPServer.Send(MSG.CFALSE, this.Socket);
                return;
            }
            if (this.rlyCmds == null)
                this.InitRlyCmds();
            if (args.Length >= 2)
                this.ProcessRlyCommand(args);
            else
                TCPServer.Send(MSG.BADARG, this.Socket);
        }
        /// <summary>
        /// Move to sch hashtable
        /// </summary>
        /// <param name="args">args</param>
        private void Root_SchCmdFunc(string[] args)
        {
            if (this.schCmds == null)
                this.InitSchCmds();
            if (args.Length >= 2)
                this.ProcessSchCommand(args);
            else
                TCPServer.Send(MSG.BADARG, this.Socket);
        }
        /// <summary>
        /// Log in as a user.
        /// Calls `ClientPermissions.Client.Login()` to validate login.
        /// </summary>
        /// <param name="args">user,pass</param>
        private void Root_LoginFunc(string[] args)
        {
            if (args.Length != 3)
            {
                lock (this._TCPLock)
                    TCPServer.Send(MSG.BADARG, this.Socket);
                return;
            }

            this.user = ClientPermissions.Client.Login(args[1], args[2]);
            if (user.group != null)
                this.group = ClientPermissions.Client.GetPermissions(user);

            if(user.username == "Guest" || this.group.groupname == "Guest")
                lock (this._TCPLock)
                    TCPServer.Send(MSG.LOGINFAIL, this.Socket);
            else
                lock (this._TCPLock)
                    TCPServer.Send(MSG.LOGINSUCCESS, this.Socket);

            
        }
        /// <summary>
        /// Returns the system satatus.
        /// displayed contents of `MainService.Status`
        /// </summary>
        /// <param name="args">none</param>
        private void Root_StatusFunc(string[] args)
        {
            lock (this._TCPLock)
                TCPServer.Send(MSG.STATUS(this.group.ToString(),clientID), this.Socket);
        }
        /// <summary>
        /// Initiate a clean disconnect.
        /// </summary>
        /// <param name="args">none</param>
        private void Root_DcCmdFunc(string[] args)
        {

        }
        /// <summary>
        /// Ask for direct control. User must have control permission. If another
        /// process or user already has control, this command will override their control
        /// without prompt if the asking user has a higher priority level.
        /// This command uses `TCPServer.TakeDirectControl()` to determine control.
        /// </summary>
        /// <param name="args">true/false</param>
        private void Root_ControlFunc(string[] args)
        {
            if (args.Length != 2 )
                lock (this._TCPLock)
                    TCPServer.Send(MSG.BADARG, this.Socket);
            else if (this.group.dControl == false)
                lock (this._TCPLock)
                    TCPServer.Send(MSG.CFAIL, this.Socket);
            else 
            {
                try
                {
                    bool b = Convert.ToBoolean(args[1]);
                    if (b) // Take control
                    {
                        if (DirectControl.TakeDirectControl(clientID, group.priority))
                            lock (this._TCPLock)
                                TCPServer.Send(MSG.CSUCCESS, this.Socket);
                        else
                            lock (this._TCPLock)
                                TCPServer.Send(MSG.CINUSE, this.Socket);

                    }
                    else // Release control
                    {
                        DirectControl.ReleaseDirectControl(clientID);
                        lock (this._TCPLock)
                            TCPServer.Send(MSG.SUCCESS, this.Socket);
                    }
                }
                catch
                {
                    lock (this._TCPLock)
                        TCPServer.Send(MSG.BADARG, this.Socket);
                    return;
                }
                
            }

        }
        /// <summary>
        /// Display the root-level help message.
        /// </summary>
        /// <param name="args">none</param>
        private void Root_HelpCmdFunc(string[] args)
        {
            lock (this._TCPLock)
                TCPServer.Send(MSG.HELP, this.Socket);
        }
        #endregion
        #region Sys level commands
        private Hashtable sysCmds;
        /// <summary>
        /// Initialize sys-level commands hashtable
        /// </summary>
        private void InitSysCmds()
        {
            Log.Instance.Write("Initializing sys cmds hashtable", Log.Priority.VERBOSE_DEBUG);
            this.sysCmds = new Hashtable();
            this.sysCmds.Add("listusers", new CommandDelegate(this.Sys_ListUsersFunc));
            this.sysCmds.Add("getuser", new CommandDelegate(this.Sys_GetUserFunc));
            this.sysCmds.Add("adduser", new CommandDelegate(this.Sys_AddUserFunc));
            this.sysCmds.Add("deleteuser", new CommandDelegate(this.Sys_DeleteUserFunc));
            this.sysCmds.Add("updateuser", new CommandDelegate(this.Sys_UpdateUserFunc));
            this.sysCmds.Add("listgroups", new CommandDelegate(this.Sys_ListGroupsFunc));
            this.sysCmds.Add("getgroup", new CommandDelegate(this.Sys_GetGroupFunc));
            this.sysCmds.Add("addgroup", new CommandDelegate(this.Sys_AddGroupFunc));
            this.sysCmds.Add("deletegroup", new CommandDelegate(this.Sys_DeleteGroupFunc));
            this.sysCmds.Add("updategroup", new CommandDelegate(this.Sys_UpdateGroupFunc));
            this.sysCmds.Add("stop", new CommandDelegate(this.Sys_StopFunc));
            this.sysCmds.Add("start", new CommandDelegate(this.Sys_StartFunc));
            this.sysCmds.Add("restart", new CommandDelegate(this.Sys_RestartFunc));
            this.sysCmds.Add("help", new CommandDelegate(this.Sys_HelpFunc));
        }
        /// <summary>
        /// Entry point for sys-level commands.
        /// Requires administration permission.
        /// </summary>
        /// <param name="args">args. Sys commands occupy args[1]</param>
        private void ProcessSysCommand(string[] args)
        {
            if (sysCmds.ContainsKey(args[1]))
                ((CommandDelegate)sysCmds[args[1]])(args);
            else
            {
                lock (this._TCPLock)
                    TCPServer.Send(MSG.SYS.BADCMD, this.Socket);
            }
        }
        /// <summary>
        /// Display the sys-level help dialog (MSG.SYS.HELP)
        /// </summary>
        /// <param name="args">none</param>
        private void Sys_HelpFunc(string[] args)
        {
            lock (this._TCPLock)
                TCPServer.Send(MSG.SYS.HELP, this.Socket);
        }
        /// <summary>
        /// Get a list of groups
        /// </summary>
        /// <param name="args">none</param>
        private void Sys_ListGroupsFunc(string[] args)
        {
            lock (this._TCPLock)
            {
                TCPServer.Send(MSG.SYS.GROUPLISTBEGIN, this.Socket);
                try
                {
                    foreach (string f in Client.listGroups())
                        TCPServer.Send(f+"\r", this.Socket);
                }
                catch { }
                finally
                {
                    TCPServer.Send(MSG.SYS.LISTEND, this.Socket);
                }
            }

        }
        /// <summary>
        /// Get a group object
        /// </summary>
        /// <param name="args">groupname</param>
        private void Sys_GetGroupFunc(string[] args)
        {
            if (args.Length != 3)
            {
                lock (this._TCPLock)
                    TCPServer.Send(MSG.BADARG, this.Socket);
                return;
            }

            Group myGroup = ClientPermissions.Client.GetGroup(args[2]);
            if (myGroup.groupname=="")
                lock (this._TCPLock)
                    TCPServer.Send(MSG.SYS.FAIL, this.Socket);
            else
                lock (this._TCPLock)
                    TCPServer.Send(MSG.SYS.GROUP + myGroup.ToString() + "\r", this.Socket);


        }
        /// <summary>
        /// Add a new user group (no overwrite)
        /// </summary>
        /// <param name="args">priority, group name, access level, direct access, scheduler access</param>
        private void Sys_AddGroupFunc(string[] args)
        {
            if (args.Length != 7)
            {
                lock (this._TCPLock)
                    TCPServer.Send(MSG.SYS.BADARG, this.Socket);
                return;
            }
            try
            {
                Group newGroup = new Group(int.Parse(args[2]),args[3],
                    (AccessLevels) Enum.Parse(typeof(AccessLevels), args[4]),
                    Convert.ToBoolean(args[5]),Convert.ToBoolean(args[6]));
                if (!Client.AddGroup(newGroup))
                {
                    lock (this._TCPLock)
                        TCPServer.Send(MSG.SYS.FAIL, this.Socket);
                    return;
                }

            }
            catch (Exception e)
            {
                Log.Instance.Write("error during add group: " + e.ToString(), Log.Priority.DEBUG);
                lock (this._TCPLock)
                    TCPServer.Send(MSG.SYS.BADARG, this.Socket);
                return;
            }
            lock (this._TCPLock)
                TCPServer.Send(MSG.SYS.SUCCESS, this.Socket);

        }
        /// <summary>
        /// update or create a user group. (overwrite)
        /// </summary>
        /// <param name="args">priority, group name, access level, direct access, scheduler access</param>
        private void Sys_UpdateGroupFunc(string[] args)
        {
            if (args.Length != 7)
            {
                lock (this._TCPLock)
                    TCPServer.Send(MSG.SYS.BADARG, this.Socket);
                return;
            }
            try
            {
                Group newGroup = new Group(int.Parse(args[2]), args[3],
                    (AccessLevels)Enum.Parse(typeof(AccessLevels), args[4]),
                    Convert.ToBoolean(args[5]), Convert.ToBoolean(args[6]));
                if (!Client.UpdateGroup(newGroup))
                {
                    lock (this._TCPLock)
                        TCPServer.Send(MSG.SYS.FAIL, this.Socket);
                    return;
                }

            }
            catch (Exception e)
            {
                Log.Instance.Write("error during update group: " + e.ToString(), Log.Priority.DEBUG);
                lock (this._TCPLock)
                    TCPServer.Send(MSG.SYS.BADARG, this.Socket);
                return;
            }
            lock (this._TCPLock)
                TCPServer.Send(MSG.SYS.SUCCESS, this.Socket);

        }
        /// <summary>
        /// Delete a group
        /// </summary>
        /// <param name="args">groupname</param>
        private void Sys_DeleteGroupFunc(string[] args)
        {
            if (args.Length != 3)
            {
                lock (this._TCPLock)
                    TCPServer.Send(MSG.SYS.BADARG, this.Socket);
                return;
            }
            if(!Client.DeleteGroup(args[2]))
                lock (this._TCPLock)
                    TCPServer.Send(MSG.SYS.FAIL, this.Socket);
            else
                lock (this._TCPLock)
                    TCPServer.Send(MSG.SYS.SUCCESS, this.Socket);

        }
        /// <summary>
        /// Get a list of users
        /// </summary>
        /// <param name="args">none</param>
        private void Sys_ListUsersFunc(string[] args)
        {
            lock (this._TCPLock)
            {
                TCPServer.Send(MSG.SYS.USERLISTBEGIN, this.Socket);
                try
                {
                    foreach (string f in Client.listUsers())
                        TCPServer.Send(f + "\r", this.Socket);
                }
                catch { }
                finally
                {
                    TCPServer.Send(MSG.SYS.LISTEND, this.Socket);
                }
            }

        }
        /// <summary>
        /// Get user object
        /// </summary>
        /// <param name="args">username</param>
        private void Sys_GetUserFunc(string[] args)
        {
            if (args.Length != 3)
            {
                lock (this._TCPLock)
                    TCPServer.Send(MSG.BADARG, this.Socket);
                return;
            }

            User myUser = ClientPermissions.Client.GetUser(args[2]);
            if (myUser.username == "Guest")
                lock (this._TCPLock)
                    TCPServer.Send(MSG.SYS.FAIL, this.Socket);
            else
                lock (this._TCPLock)
                    TCPServer.Send(MSG.SYS.USER +myUser.ToString()+"\r", this.Socket);


        }
        /// <summary>
        /// Add a user (no overwrite)
        /// </summary>
        /// <param name="args">username, password, groupname</param>
        private void Sys_AddUserFunc(string[] args)
        {
            if (args.Length != 5)
            {
                lock (this._TCPLock)
                    TCPServer.Send(MSG.SYS.BADARG, this.Socket);
                return;
            }
            try
            {
                User newUser = new User(args[2], args[3], args[4]);
                if (Client.GetPermissions(newUser).groupname == "Guest")
                {
                    lock (this._TCPLock)
                        TCPServer.Send(MSG.SYS.BADGROUP, this.Socket);
                    return;
                }
                if (!Client.AddUser(newUser))
                {
                    lock (this._TCPLock)
                        TCPServer.Send(MSG.SYS.FAIL, this.Socket);
                    return;
                }
            }
            catch (Exception e)
            {
                Log.Instance.Write("error during add user: " + e.ToString(), Log.Priority.DEBUG);
                lock (this._TCPLock)
                    TCPServer.Send(MSG.SYS.BADARG, this.Socket);
                return;
            }
            lock (this._TCPLock)
                TCPServer.Send(MSG.SYS.SUCCESS, this.Socket);

        }
        /// <summary>
        /// Update a user (overwrite)
        /// </summary>
        /// <param name="args">username, password, groupname</param>
        private void Sys_UpdateUserFunc(string[] args)
        {
            if (args.Length != 5)
            {
                lock (this._TCPLock)
                    TCPServer.Send(MSG.SYS.BADARG, this.Socket);
                return;
            }
            try
            {
                User newUser = Client.GetUser(args[2]);
                newUser.group = args[4];   
                if (Client.GetPermissions(newUser).groupname == "Guest")
                {
                    lock (this._TCPLock)
                        TCPServer.Send(MSG.SYS.BADUSER, this.Socket);
                    return;
                }
                if(args[3] != "") //Generate MD5 pass hash if new pass is defined
                    using (MD5 md5Hash = MD5.Create())
                        newUser.password = Convert.ToBase64String(md5Hash.ComputeHash(Encoding.UTF8.GetBytes(args[3])));
                if (!Client.UpdateUser(newUser))
                {
                    lock (this._TCPLock)
                        TCPServer.Send(MSG.SYS.FAIL, this.Socket);
                    return;
                }
            }
            catch (Exception e)
            {
                Log.Instance.Write("error during add user: " + e.ToString(), Log.Priority.DEBUG);
                lock (this._TCPLock)
                    TCPServer.Send(MSG.SYS.BADARG, this.Socket);
                return;
            }
            lock (this._TCPLock)
                TCPServer.Send(MSG.SYS.SUCCESS, this.Socket);

        }
        /// <summary>
        /// Delete a user
        /// </summary>
        /// <param name="args">username</param>
        private void Sys_DeleteUserFunc(string[] args)
        {
            if (args.Length != 3)
            {
                lock (this._TCPLock)
                    TCPServer.Send(MSG.SYS.BADARG, this.Socket);
                return;
            }
            if(!Client.DeleteUser(args[2]))
                lock (this._TCPLock)
                    TCPServer.Send(MSG.SYS.FAIL, this.Socket);
            else
                lock (this._TCPLock)
                    TCPServer.Send(MSG.SYS.SUCCESS, this.Socket);

        }
        /// <summary>
        /// Start a module.
        /// </summary>
        /// <param name="args">module</param>
        private void Sys_StartFunc(string[] args)
        {
            if (args.Length != 3)
            {
                lock (this._TCPLock)
                    TCPServer.Send(MSG.BADARG, this.Socket);
                return;
            }
            switch (args[2])
            {
                case "all":
                    MainService.cameraSystems[0].camInst.Start();
                    MainService.cameraSystems[0].ptInst.Start();
                    MainService.cameraSystems[0].digInst.Start();
                    Scheduler.StartTaskScheduler();
                    break;
                case "sch":
                    Scheduler.StartTaskScheduler();
                    break;
                case "cam":
                    MainService.cameraSystems[0].camInst.Start();
                    break;
                case "rly":
                    MainService.cameraSystems[0].digInst.Start();
                    break;
                case "pt":
                    MainService.cameraSystems[0].ptInst.Start();
                    break;
                default:
                    TCPServer.Send(MSG.BADARG, this.Socket);
                    return;
                    
            }
            TCPServer.Send(MSG.SYS.OK, this.Socket);

        }
        /// <summary>
        /// Stop a module.
        /// </summary>
        /// <param name="args">module</param>
        private void Sys_StopFunc(string[] args)
        {
            if (args.Length != 3)
            {
                lock (this._TCPLock)
                    TCPServer.Send(MSG.BADARG, this.Socket);
                return;
            }
            switch (args[2])
            {
                case "all":
                    MainService.cameraSystems[0].camInst.Stop();
                    MainService.cameraSystems[0].ptInst.Stop();
                    MainService.cameraSystems[0].digInst.Stop();
                    Scheduler.StopTaskScheduler();
                    break;
                case "sch":
                    Scheduler.StopTaskScheduler();
                    break;
                case "cam":
                    MainService.cameraSystems[0].camInst.Stop();
                    break;
                case "rly":
                    MainService.cameraSystems[0].digInst.Stop();
                    break;
                case "pt":
                    MainService.cameraSystems[0].ptInst.Stop();
                    break;
                default:
                    TCPServer.Send(MSG.BADARG, this.Socket);
                    return;   
            }
            TCPServer.Send(MSG.SYS.OK, this.Socket);

        }
        /// <summary>
        /// Restart a module.
        /// </summary>
        /// <param name="args">module</param>
        private void Sys_RestartFunc(string[] args)
        {
            if (args.Length != 3)
            {
                lock (this._TCPLock)
                    TCPServer.Send(MSG.BADARG, this.Socket);
                return;
            }
            switch (args[2])
            {
                case "all":
                    lock (this._TCPLock)
                        TCPServer.BroadcastToClients(MSG.SYS.RESTART);
                    new Thread(new ThreadStart(MainService.This.Restart)).Start();
                    break;
                case "sch":
                    Scheduler.StopTaskScheduler();
                    Scheduler.StartTaskScheduler();
                    break;
                case "cam":
                    MainService.cameraSystems[0].camInst.Stop();
                    MainService.cameraSystems[0].camInst.Start();
                    break;
                case "rly":
                    MainService.cameraSystems[0].digInst.Stop();
                    MainService.cameraSystems[0].digInst.Start();
                    break;
                case "pt":
                    MainService.cameraSystems[0].ptInst.Stop();
                    MainService.cameraSystems[0].ptInst.Start();
                    break;
                default:
                    TCPServer.Send(MSG.BADARG, this.Socket);
                    return;
            }
            TCPServer.Send(MSG.SYS.OK, this.Socket);

        }
        #endregion
        #region Usr level commands
        private Hashtable usrCmds;
        /// <summary>
        /// Initialize usr-level commands
        /// </summary>
        private void InitUsrCmds()
        {
            Log.Instance.Write("Initializing usr cmds hashtable", Log.Priority.VERBOSE_DEBUG);
            this.usrCmds = new Hashtable();
            this.usrCmds.Add("createpreset", new CommandDelegate(this.Usr_CreatePresetFunc));
            this.usrCmds.Add("loadpreset", new CommandDelegate(this.Usr_LoadPresetFunc));
            this.usrCmds.Add("deletepreset", new CommandDelegate(this.Usr_DeletePresetFunc));
            this.usrCmds.Add("showpreset", new CommandDelegate(this.Usr_ShowPresetFunc));
            this.usrCmds.Add("listpresets", new CommandDelegate(this.Usr_ListPresetsFunc));
            this.usrCmds.Add("help", new CommandDelegate(this.Usr_HelpFunc));

        }
        /// <summary>
        /// Process a user-level command. 
        /// </summary>
        /// <param name="args">args. usr commmand occupies args[1]</param>
        private void ProcessUsrCommand(string[] args)
        {
                ((CommandDelegate)usrCmds[args[1]])(args);
        }
        /// <summary>
        /// Create a preset file for the current user
        /// </summary>
        /// <param name="args">name,res,mflash,mfocus,expMode,metering,whitebal,aperture,expTime,expBias,zoom,focus,iso,x,y</param>
        private void Usr_CreatePresetFunc(string[] args)
        {
            if (args.Length != 21)
            {
                lock (this._TCPLock)
                    TCPServer.Send(MSG.BADARG, this.Socket);
                return;
            }
            Preset preset = new Preset();
            try
            {
                if (!MainService.cameraSystems[activeCamSys].camInst.listSettingChoices("res").Contains(args[3]) ||
                    !MainService.cameraSystems[activeCamSys].camInst.listSettingChoices("mflash").Contains(args[4]) ||
                    !MainService.cameraSystems[activeCamSys].camInst.listSettingChoices("mfocus").Contains(args[5]) ||
                    !MainService.cameraSystems[activeCamSys].camInst.listSettingChoices("emode").Contains(args[6]) ||
                    !MainService.cameraSystems[activeCamSys].camInst.listSettingChoices("met").Contains(args[7]) ||
                    !MainService.cameraSystems[activeCamSys].camInst.listSettingChoices("wb").Contains(args[8]) ||
                    !MainService.cameraSystems[activeCamSys].camInst.listSettingChoices("iso").Contains(args[14]))
                        throw new ArgumentException("Member not in enum.");

                preset.camSetting.resolution    = args[3];
                preset.camSetting.flashMode     = args[4];
                preset.camSetting.focusMode     = args[5];
                preset.camSetting.expMode       = args[6];
                preset.camSetting.metering      = args[7];
                preset.camSetting.whiteBal      = args[8];
                preset.camSetting.aperture      = int.Parse(args[9]);
                preset.camSetting.expTime       = int.Parse(args[10]);
                preset.camSetting.expBias       = int.Parse(args[11]);
                preset.camSetting.zoom          = int.Parse(args[12]);
                preset.camSetting.focus         = int.Parse(args[13]);
                preset.camSetting.iso           = args[14];
                preset.ptX                      = int.Parse(args[15]);
                preset.ptY                      = int.Parse(args[16]);
                preset.rly                      = new bool[] { Boolean.Parse(args[17]), Boolean.Parse(args[18]), Boolean.Parse(args[19]), Boolean.Parse(args[20]) };

                if (Presets.SavePreset(this.user.username + "_" + args[2], preset))
                    lock (this._TCPLock)
                        TCPServer.Send(MSG.SUCCESS, this.Socket);
                else
                    TCPServer.Send(MSG.USR.FAIL, this.Socket);
            }
            catch (Exception e)
            {
                Log.Instance.Write(e.ToString(), Log.Priority.VERBOSE_DEBUG);
                lock (this._TCPLock)
                    TCPServer.Send(MSG.BADARG, this.Socket);
                return;
            }
           
        }
        /// <summary>
        /// Load a preset from a file
        /// </summary>
        /// <param name="args">preset name</param>
        private void Usr_LoadPresetFunc(string[] args)
        {
            if (args.Length != 3)
            {
                lock (this._TCPLock)
                    TCPServer.Send(MSG.BADARG, this.Socket);
                return;
            }
            if (Presets.SetPreset(this.user.username + "_" + args[2]))
                lock (this._TCPLock)
                    TCPServer.Send(MSG.SUCCESS, this.Socket);
            else
                TCPServer.Send(MSG.USR.FAIL, this.Socket);
        }
        /// <summary>
        /// Respond with a string representing a preset file.
        /// </summary>
        /// <param name="args">preset filename</param>
        private void Usr_ShowPresetFunc(string[] args)
        {
            if (args.Length != 3)
            {
                lock (this._TCPLock)
                    TCPServer.Send(MSG.BADARG, this.Socket);
                return;
            }
            lock (this._TCPLock)
            {
                string preset = Presets.ToString(this.user.username + "_" + args[2]);
                if (preset == null)
                    TCPServer.Send(MSG.USR.FAIL, this.Socket);
                else
                    TCPServer.Send(MSG.USR.PRESET + args[2] + "=" + preset, this.Socket);
            }
        }
        /// <summary>
        /// Delete a preset file owned by this user.
        /// </summary>
        /// <param name="args">settings name</param>
        private void Usr_DeletePresetFunc(string[] args)
        {
            if (Presets.DeletePreset(this.user.username + "_" + args[2]))
                lock (this._TCPLock)
                    TCPServer.Send(MSG.SUCCESS, this.Socket);
            else
                TCPServer.Send(MSG.USR.FAIL, this.Socket);
        }
        /// <summary>
        /// List all preset files the current user owns.
        /// </summary>
        /// <param name="args">none</param>
        private void Usr_ListPresetsFunc(string[] args)
        {
            lock (this._TCPLock)
            {
                TCPServer.Send(MSG.USR.PRESETLISTBEGIN, this.Socket);
                try
                {
                    foreach (string f in Presets.listSettings(this.user.username))
                        TCPServer.Send(f.Replace(this.user.username + "_","") + "\r", this.Socket);
                }catch{}finally {
                    TCPServer.Send(MSG.USR.LISTEND, this.Socket);
                }
            }
        }
        /// <summary>
        /// Display the usr-level help menu.
        /// </summary>
        /// <param name="args">none</param>
        private void Usr_HelpFunc(string[] args)
        {
            lock (this._TCPLock)
                TCPServer.Send(MSG.USR.HELP, this.Socket);
        }
        #endregion
        #region Cam level commands
        private Hashtable camCmds;
        /// <summary>
        /// Initialize cam-level commands
        /// </summary>
        private void InitCamCmds()
        {
            Log.Instance.Write("Initializing cam cmds hashtable", Log.Priority.VERBOSE_DEBUG);
            this.camCmds = new Hashtable();
            this.camCmds.Add("takephoto", new CommandDelegate(this.Cam_TakePictureFunc));
            this.camCmds.Add("list", new CommandDelegate(this.Cam_ListFunc));
            this.camCmds.Add("set", new CommandDelegate(this.Cam_SetFunc));
            this.camCmds.Add("get", new CommandDelegate(this.Cam_GetFunc));
            this.camCmds.Add("help", new CommandDelegate(this.Cam_HelpFunc));

        }
        /// <summary>
        /// Process a cam-level commands. 
        /// </summary>
        /// <param name="args">args. Cam commmand occupies args[1]</param>
        private void ProcessCamCommand(string[] args)
        {
            if (MainService.status.camera == Status.ONLINE)
                ((CommandDelegate)camCmds[args[1]])(args);
            else
                lock (this._TCPLock)
                    TCPServer.Send(MSG.CAM.NC, this.Socket);
        }
        /// <summary>
        /// Take a photo. Blocks until complete and sends success/failure notification.
        /// </summary>
        /// <param name="args">none</param>
        private void Cam_TakePictureFunc(string[] args)
        {
            TCPServer.Send(MSG.CAM.PHOTOSTART, this.Socket);
            if (MainService.cameraSystems[activeCamSys].camInst.TakePhoto() != null)
                lock (this._TCPLock)
                    TCPServer.Send(MSG.CAM.PHOTOSUCCESS, this.Socket);
            else
                TCPServer.Send(MSG.CAM.PHOTOFAIL, this.Socket);
        }
        /// <summary>
        /// List all possible flash modes for the current camera.
        /// </summary>
        /// <param name="args">none</param>
        private void Cam_ListFunc(string[] args)
        {
            lock (this._TCPLock)
            {
                TCPServer.Send(MSG.CAM.LISTBEGIN + args[2] + "\r", this.Socket);
                try{
                    foreach (string f in MainService.cameraSystems[activeCamSys].camInst.listSettingChoices(args[2]))
                        TCPServer.Send(f + "\r", this.Socket);
                }catch{}finally {
                    TCPServer.Send(MSG.CAM.LISTEND, this.Socket);
                }
            }
        } 
        /// <summary>
        /// List all possible flash modes for the current camera.
        /// </summary>
        /// <param name="args">none</param>
        private void Cam_GetFunc(string[] args)
        {
            if (args.Length != 3)
            {
                lock (this._TCPLock)
                    TCPServer.Send(MSG.BADARG, this.Socket);
                return;
            }
            lock (this._TCPLock)
            {
                string s = MainService.cameraSystems[activeCamSys].camInst.GetSetting(args[2]);
                if (s != null)
                    TCPServer.Send(MSG.CAM.SETTING + args[2] + "=" + s, this.Socket);
                else
                    TCPServer.Send(MSG.BADARG, this.Socket);
            }
        }
        /// <summary>
        /// Set the zoom level.
        /// </summary>
        /// <param name="args">zoom</param>
        private void Cam_SetFunc(string[] args)
        {
            if (args.Length != 4)
            {
                lock (this._TCPLock)
                    TCPServer.Send(MSG.BADARG, this.Socket);
                return;
            }
            try
            {
                if(MainService.cameraSystems[activeCamSys].camInst.SetSetting(args[2],args[3]))
                    lock (this._TCPLock)
                        TCPServer.Send(MSG.SUCCESS, this.Socket);
                else
                    lock (this._TCPLock)
                        TCPServer.Send(MSG.CAM.FAIL, this.Socket);
            }
            catch
            {
                lock (this._TCPLock)
                    TCPServer.Send(MSG.BADARG, this.Socket);
            }

        }
        /// <summary>
        /// Display the cam-level help menu.
        /// </summary>
        /// <param name="args">none</param>
        private void Cam_HelpFunc(string[] args)
        {
            lock (this._TCPLock)
                TCPServer.Send(MSG.CAM.HELP, this.Socket);
        }
        #endregion
        #region Relay level commands
        private Hashtable rlyCmds;
        /// <summary>
        /// Initialize rly-level commands
        /// </summary>
        private void InitRlyCmds()
        {
            Log.Instance.Write("Initializing rly cmds hashtable", Log.Priority.VERBOSE_DEBUG);
            this.rlyCmds = new Hashtable();
            this.rlyCmds.Add("set", new CommandDelegate(this.Rly_SetFunc));
            this.rlyCmds.Add("get", new CommandDelegate(this.Rly_GetFunc));
            this.rlyCmds.Add("list", new CommandDelegate(this.Rly_ListFunc));
            this.rlyCmds.Add("help", new CommandDelegate(this.Rly_HelpFunc));

        }
        /// <summary>
        /// Process rly-level commands. Direct control required
        /// </summary>
        /// <param name="args">args. Command parsed at args[1] position.</param>
        private void ProcessRlyCommand(string[] args)
        {
            if (MainService.status.relays == Status.ONLINE)
                ((CommandDelegate)rlyCmds[args[1]])(args);
            else
                lock (this._TCPLock)
                    TCPServer.Send(MSG.RLY.NC, this.Socket);
        }
        /// <summary>
        /// Set specified relay channel to state given
        /// </summary>
        /// <param name="args">channel,state</param>
        private void Rly_SetFunc(string[] args)
        {
            bool val;
            int chan;
            try
            {
                val = Convert.ToBoolean(args[3]);
                chan = int.Parse(args[2]);
            }
            catch
            {
                lock (this._TCPLock)
                    TCPServer.Send(MSG.RLY.BADARG, this.Socket);
                return;
            }

            if (MainService.cameraSystems[activeCamSys].digInst.SetOutput(chan, val))
                lock (this._TCPLock)
                    TCPServer.Send(MSG.RLY.SUCCESS, this.Socket);
            else
                lock (this._TCPLock)
                    TCPServer.Send(MSG.RLY.FAIL, this.Socket);
        }
        /// <summary>
        /// Get the state of the specified relay channel.
        /// </summary>
        /// <param name="args">channel</param>
        private void Rly_GetFunc(string[] args)
        {
            int chan;
            try
            {
                chan = int.Parse(args[2]);
            }
            catch
            {
                lock (this._TCPLock)
                    TCPServer.Send(MSG.RLY.BADARG, this.Socket);
                return;
            }
            if(chan>3 || chan <0)
                lock (this._TCPLock)
                    TCPServer.Send(MSG.RLY.BADARG, this.Socket);
            else
                lock (this._TCPLock)
                    TCPServer.Send(MSG.RLY.GET(chan, MainService.cameraSystems[activeCamSys].digInst.GetOutput(chan)), this.Socket);
        }
        /// <summary>
        /// Return a list of channel names
        /// </summary>
        /// <param name="args">none</param>
        private void Rly_ListFunc(string[] args)
        {
            lock (this._TCPLock)
            {
                TCPServer.Send(MSG.RLY.LISTBEGIN, this.Socket);
                try
                {
                    foreach (string f in MainService.cameraSystems[activeCamSys].digInst.listOutputs())
                        TCPServer.Send(f + "\r", this.Socket);
                }
                catch { }
                finally
                {
                    TCPServer.Send(MSG.RLY.LISTEND, this.Socket);
                }
            }
        }
        /// <summary>
        /// Rly-level help message
        /// </summary>
        /// <param name="args"></param>
        private void Rly_HelpFunc(string[] args)
        {
            lock (this._TCPLock)
                TCPServer.Send(MSG.RLY.HELP, this.Socket);
        }
        #endregion
        #region Scheduler level commands
        private Hashtable schCmds;
        /// <summary>
        /// Initialize scheduler commands
        /// </summary>
        private void InitSchCmds()
        {
            Log.Instance.Write("Initializing sch cmds hashtable", Log.Priority.VERBOSE_DEBUG);
            this.schCmds = new Hashtable();
            this.schCmds.Add("takephoto", new CommandDelegate(this.Sch_TakePhotoAtFunc));
            this.schCmds.Add("list", new CommandDelegate(this.Sch_ListFunc));
            this.schCmds.Add("delete", new CommandDelegate(this.Sch_DeleteFunc));
            this.schCmds.Add("help", new CommandDelegate(this.Sch_HelpFunc));

        }
        /// <summary>
        /// Process scheduler commands
        /// </summary>
        /// <param name="args">command at args[1]</param>
        private void ProcessSchCommand(string[] args)
        {
            if (MainService.status.scheduler == Status.ONLINE)
                ((CommandDelegate)schCmds[args[1]])(args);
            else
                lock (this._TCPLock)
                    TCPServer.Send(MSG.SCH.NC, this.Socket);
        }
        /// <summary>
        /// add a "takephotoat" event.
        /// </summary>
        /// <param name="args">settings,x,y,cron?,time/crontab</param>
        private void Sch_TakePhotoAtFunc(string[] args)
        {
            if (!this.group.sControl)
            {
                lock (_TCPLock)
                    TCPServer.Send(MSG.PFAIL, this.Socket);
                return;
            }
            if (args.Length != 6)
            {
                TCPServer.Send(MSG.SCH.BADARG, this.Socket);
                return;
            }
            bool b;
            try
            {
                b = Convert.ToBoolean(args[3]);
                if (!b)
                    Convert.ToDateTime(args[4]);
            }
            catch
            {
                lock (this._TCPLock)
                    TCPServer.Send(MSG.SCH.BADARG, this.Socket);
                return;
            }

            string s = args[2];
            if(Scheduler.CreateJob(DateTime.Now.ToString("yyMMdd HHmmss - ") + args[5],"TakePhoto", this.user.username, s, b, args[4]))
                lock (this._TCPLock)
                    TCPServer.Send(MSG.SCH.SUCCESS, this.Socket);
            else
            lock (this._TCPLock)
                    TCPServer.Send(MSG.SCH.FAIL, this.Socket);
        }
        /// <summary>
        /// List all scheduled jobs
        /// </summary>
        /// <param name="args">none</param>
        private void Sch_ListFunc(string[] args)
        {
            lock (this._TCPLock)
            {
                TCPServer.Send(MSG.SCH.LISTBEGIN, this.Socket);
                foreach (string f in Scheduler.GetAllJobs())
                    TCPServer.Send(f + "\r", this.Socket);
                TCPServer.Send(MSG.SCH.LISTEND, this.Socket);
            }
        }
        /// <summary>
        /// Delete a job from the scheduler
        /// </summary>
        /// <param name="args">jobname,owner</param>
        private void Sch_DeleteFunc(string[] args)
        {
            if (!this.group.sControl)
            {
                lock (_TCPLock)
                    TCPServer.Send(MSG.PFAIL, this.Socket);
                return;
            }
             if (args.Length != 4)
            {
                TCPServer.Send(MSG.SCH.BADARG, this.Socket);
                return;
            }
            lock (this._TCPLock)
            {
                if(Scheduler.DeleteJob(args[2],args[3]))
                    TCPServer.Send(MSG.SCH.SUCCESS, this.Socket);
                else
                    TCPServer.Send(MSG.SCH.FAIL, this.Socket);
            }
        }
        /// <summary>
        /// Display scheduler-level help message
        /// </summary>
        /// <param name="args">none</param>
        private void Sch_HelpFunc(string[] args)
        {
            lock (this._TCPLock)
                TCPServer.Send(MSG.RLY.HELP, this.Socket);
        }
        #endregion
        #region Pan/Tilt level commands
        private Hashtable ptCmds;
        /// <summary>
        /// Initialize pantilt-level commands
        /// </summary>
        private void InitPtCmds()
        {
            Log.Instance.Write("Initializing pt cmds hashtable", Log.Priority.VERBOSE_DEBUG);
            this.ptCmds = new Hashtable();
            this.ptCmds.Add("move", new CommandDelegate(this.Pt_MoveFunc));
            this.ptCmds.Add("brake", new CommandDelegate(this.Pt_BrakeFunc));
            this.ptCmds.Add("moveto", new CommandDelegate(this.Pt_MoveToFunc));
            this.ptCmds.Add("movetox", new CommandDelegate(this.Pt_MoveToXFunc));
            this.ptCmds.Add("movetoy", new CommandDelegate(this.Pt_MoveToYFunc));
            this.ptCmds.Add("moverel", new CommandDelegate(this.Pt_MoveRelFunc));
            this.ptCmds.Add("panmax", new CommandDelegate(this.Pt_PanSpeedFunc));
            this.ptCmds.Add("tiltmax", new CommandDelegate(this.Pt_TiltSpeedFunc));
            this.ptCmds.Add("stop", new CommandDelegate(this.Pt_StopFunc));
            this.ptCmds.Add("getpos", new CommandDelegate(this.Pt_GetPosFunc));
            this.ptCmds.Add("help", new CommandDelegate(this.Pt_HelpFunc));

        }
        /// <summary>
        /// Process a pan and tilt command.
        /// </summary>
        /// <param name="args">args[1] contains command</param>
        private void ProcessPtCommand(string[] args)
        {
            if (MainService.status.pt == Status.ONLINE) // Fix potential for invalid arguement exception
                ((CommandDelegate)ptCmds[args[1]])(args);
            else
                lock (this._TCPLock)
                    TCPServer.Send(MSG.PT.NC, this.Socket);
        }
        /// <summary>
        /// Move in direction at speed
        /// </summary>
        /// <param name="args">direction,speed</param>
        private void Pt_MoveFunc(string[] args)
        {
            string dir;
            int speed;
            try
            {
                dir = args[2].ToUpper();
                speed = int.Parse(args[3]);
                if (speed < 0 || speed > 100)
                    throw new ArgumentException("Speed out of range.");
                else if (dir != "L" && dir != "R" && dir != "U" && dir != "D")
                    throw new ArgumentException("Invalid direction.");
            }
            catch
            {
                lock (this._TCPLock)
                    TCPServer.Send(MSG.PT.BADARG, this.Socket);
                return;
            }

            if (MainService.cameraSystems[activeCamSys].ptInst.Move(dir,speed))
                lock (this._TCPLock)
                    TCPServer.Send(MSG.PT.SUCCESS, this.Socket);
            else
                lock (this._TCPLock)
                    TCPServer.Send(MSG.PT.FAIL, this.Socket);
        }
        /// <summary>
        /// Move to an x,y location
        /// </summary>
        /// <param name="args">x,y</param>
        private void Pt_MoveToFunc(string[] args)
        {
            int x, y;
            try
            {
                x = int.Parse(args[2]);
                y = int.Parse(args[3]);
            }
            catch
            {
                lock (this._TCPLock)
                    TCPServer.Send(MSG.PT.BADARG, this.Socket);
                return;
            }

            if (MainService.cameraSystems[activeCamSys].ptInst.MoveTo(x,y))
                lock (this._TCPLock)
                    TCPServer.Send(MSG.PT.SUCCESS, this.Socket);
            else
                lock (this._TCPLock)
                    TCPServer.Send(MSG.PT.FAIL, this.Socket);
        }
        /// <summary>
        /// make a relative move of x,y units 
        /// </summary>
        /// <param name="args">x,y</param>
        private void Pt_MoveRelFunc(string[] args)
        {
            int x, y;
            try
            {
                x = int.Parse(args[2]);
                y = int.Parse(args[3]);
            }
            catch
            {
                lock (this._TCPLock)
                    TCPServer.Send(MSG.PT.BADARG, this.Socket);
                return;
            }

            if (MainService.cameraSystems[activeCamSys].ptInst.MoveRel(x, y))
                lock (this._TCPLock)
                    TCPServer.Send(MSG.PT.SUCCESS, this.Socket);
            else
                lock (this._TCPLock)
                    TCPServer.Send(MSG.PT.FAIL, this.Socket);
        }
        /// <summary>
        /// Move to the given x coordinate
        /// </summary>
        /// <param name="args">x</param>
        private void Pt_MoveToXFunc(string[] args)
        {
            int x;
            try
            {
                x = int.Parse(args[2]);
            }
            catch
            {
                lock (this._TCPLock)
                    TCPServer.Send(MSG.PT.BADARG, this.Socket);
                return;
            }
            if (MainService.cameraSystems[activeCamSys].ptInst.MoveX(x))
                lock (this._TCPLock)
                    TCPServer.Send(MSG.PT.SUCCESS, this.Socket);
            else
                lock (this._TCPLock)
                    TCPServer.Send(MSG.PT.FAIL, this.Socket);
        }
        /// <summary>
        /// Move to the given y coordinate.
        /// </summary>
        /// <param name="args">y</param>
        private void Pt_MoveToYFunc(string[] args)
        {
            int y;
            try
            {
                y = int.Parse(args[2]);
            }
            catch
            {
                lock (this._TCPLock)
                    TCPServer.Send(MSG.PT.BADARG, this.Socket);
                return;
            }
            if (MainService.cameraSystems[activeCamSys].ptInst.MoveY(y))
                lock (this._TCPLock)
                    TCPServer.Send(MSG.PT.SUCCESS, this.Socket);
            else
                lock (this._TCPLock)
                    TCPServer.Send(MSG.PT.FAIL, this.Socket);
        }
        /// <summary>
        /// Set the pan speed for absolute/relative movements
        /// </summary>
        /// <param name="args">speed</param>
        private void Pt_PanSpeedFunc(string[] args)
        {
            int speed;
            try
            {
                speed = int.Parse(args[2]);
            }
            catch
            {
                lock (this._TCPLock)
                    TCPServer.Send(MSG.PT.BADARG, this.Socket);
                return;
            }
            if (MainService.cameraSystems[activeCamSys].ptInst.PanSpeed(speed))
                lock (this._TCPLock)
                    TCPServer.Send(MSG.PT.SUCCESS, this.Socket);
            else
                lock (this._TCPLock)
                    TCPServer.Send(MSG.PT.FAIL, this.Socket);
        }
        /// <summary>
        /// Set the tilt speed for absolute/relative movements
        /// </summary>
        /// <param name="args">speed</param>
        private void Pt_TiltSpeedFunc(string[] args)
        {
            int speed;
            try
            {
                speed = int.Parse(args[2]);
            }
            catch
            {
                lock (this._TCPLock)
                    TCPServer.Send(MSG.PT.BADARG, this.Socket);
                return;
            }
            if (MainService.cameraSystems[activeCamSys].ptInst.TiltSpeed(speed))
                lock (this._TCPLock)
                    TCPServer.Send(MSG.PT.SUCCESS, this.Socket);
            else
                lock (this._TCPLock)
                    TCPServer.Send(MSG.PT.FAIL, this.Socket);
        }
        /// <summary>
        /// Turn on/off the brake
        /// </summary>
        /// <param name="args">bool</param>
        private void Pt_BrakeFunc(string[] args)
        {
            bool b;
            try
            {
                b = Convert.ToBoolean(args[2]);
            }
            catch
            {
                lock (this._TCPLock)
                    TCPServer.Send(MSG.PT.BADARG, this.Socket);
                return;
            }
            if (MainService.cameraSystems[activeCamSys].ptInst.Brake(b))
                lock (this._TCPLock)
                    TCPServer.Send(MSG.PT.SUCCESS, this.Socket);
            else
                lock (this._TCPLock)
                    TCPServer.Send(MSG.PT.FAIL, this.Socket);
        }
        /// <summary>
        /// Stop the PT unit and cancel all commands.
        /// </summary>
        /// <param name="args">none</param>
        private void Pt_StopFunc(string[] args)
        {
            if (MainService.cameraSystems[activeCamSys].ptInst.MoveStop())
                lock (this._TCPLock)
                    TCPServer.Send(MSG.PT.SUCCESS, this.Socket);
            else
                lock (this._TCPLock)
                    TCPServer.Send(MSG.PT.FAIL, this.Socket);
        }
        /// <summary>
        /// Get the current pt position.
        /// </summary>
        /// <param name="args">none</param>
        private void Pt_GetPosFunc(string[] args)
        {
            PanTilt.Pos cpos;
            cpos = MainService.cameraSystems[activeCamSys].ptInst.GetPos();
            lock (this._TCPLock)
                TCPServer.Send(MSG.PT.GETPOS(cpos.x, cpos.y), this.Socket);
        }
        /// <summary>
        /// Display the pt-level help message.
        /// </summary>
        /// <param name="args">none</param>
        private void Pt_HelpFunc(string[] args)
        {
            lock (this._TCPLock)
                TCPServer.Send(MSG.PT.HELP, this.Socket);
        }
        #endregion
    }
}
