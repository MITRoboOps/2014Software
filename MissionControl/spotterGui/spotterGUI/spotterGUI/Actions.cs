using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
//! \file Actions.cs Actions data structures, for watchdogs. 
/* 
 * Contact: Reo - reobaird@gmail.com
 * Revision: 1
 * Date: March 22th
 * 
 */

namespace DSC
{
    /// <summary>
    /// Device/service status enumeration. 
    /// 
    /// Each component (ie: TCP server, scheduler, camera, etc.) gets a status 
    /// flag noting its current status. This flag is visible to clients via the
    /// "status" command. It is also used by watchdog processes to detect faults.
    /// </summary>
    public enum Status { ONLINE, OFFLINE, ERROR, STARTING, SHUTDOWN}
    /// <summary>
    /// Action type enumeration.
    /// 
    /// The action type enum is relevant to the action item class.
    /// As part of the actions framework, the ActionType tells
    /// action workers what sort of action flag was raised.
    /// For example, raising an abort flag will signal the
    /// targetted worker or watchdog thread to terminate.
    /// </summary>
    public enum ActionType { CLIENTMSG, TIMER, ABORT };
    /// <summary>
    /// The ActionItem class represents a single action item.
    /// 
    /// ActionItem is used as the type for action queues in 
    /// classes that have watchdog threads (ie: ClientMain, etc.)
    /// </summary>
    public class ActionItem
    {

        public readonly ActionType actionType;
        public readonly string args;
        public readonly int clientID;
        
        /// <summary>
        /// Construct a new ActionItem
        /// </summary>
        /// <param name="at">Action Type</param>
        /// <param name="args">Arguements</param>
        /// <param name="id">Client ID</param>
        public ActionItem(ActionType at, string args, int id)
        {
            this.clientID = id;
            this.args = args;
            this.actionType = at;
        }
        /// <summary>
        /// Construct a new ActionItem
        /// </summary>
        /// <param name="at">Action Type</param>
        /// <param name="id">Client ID</param>
        public ActionItem(ActionType at, int id)
        {
            this.clientID = id;
            this.actionType = at;
        }
        /// <summary>
        /// Construct a new ActionItem
        /// </summary>
        /// <param name="at">Action Type</param>
        /// <param name="args">Arguements</param>
        public ActionItem(ActionType at, string args)
        {
            this.args = args;
            this.actionType = at;
        }
        /// <summary>
        /// Construct a new ActionItem
        /// </summary>
        /// <param name="at">Action Type</param>
        public ActionItem(ActionType at)
        {
            this.args = String.Empty;
            this.actionType = at;
        }
    }
}
